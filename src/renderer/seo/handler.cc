// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/handler.h"

#include <iostream>

#include <glog/logging.h>

#include "include/base/cef_bind.h"

#include "base/util.h"
#include "include/cef_app.h"
#include "proto/seo/response.pb.h"
#include "renderer/common/protobufs.h"
#include "renderer/common/visitors.h"
#include "renderer/common/tasks.h"


namespace seo {

namespace {

Handler* g_instance = NULL;

}  // namespace


Handler::Handler(CefRefPtr<common::RenderHandler> render_handler,
                 CefRefPtr<RequestHandler> request_handler)
: render_handler_(render_handler), request_handler_(request_handler),
  exit_(false), pending_(0), output_stream_(STDOUT_FILENO)
{
  ASSERT(!g_instance);
  g_instance = this;
}

Handler::~Handler() {
  g_instance = NULL;
}

Handler* Handler::GetInstance() {
  return g_instance;
}

void Handler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                   bool isLoading, bool canGoBack,
                                   bool canGoForward) {
  REQUIRE_UI_THREAD();

  if (!isLoading) {
    LOG(INFO) << "page loaded: " << browser->GetMainFrame()->GetURL().ToString();

    CefRefPtr<CefTask> task = common::TaskFromCallback(
        base::Bind(&Handler::GetSourceCodeDelayed_, this, browser));
    CefPostDelayedTask(TID_UI, task, 500);
  }
}

void Handler::OnLoadError(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          ErrorCode errorCode, const CefString& errorText,
                          const CefString& failedUrl) {
  REQUIRE_UI_THREAD();

  if (errorCode == ERR_CONNECTION_REFUSED) {
    LOG(FATAL) << "cannot connect to " << failedUrl.ToString();
  }

  LOG(FATAL) << "error loading (" << errorCode << "): " << errorText.ToString();
}


void Handler::CountPendingRequest() {
  base::AutoLock lock_scope(pending_lock_);
  pending_++;
}


void Handler::Exit() {
  base::AutoLock lock_scope(pending_lock_);

  if (pending_ == 0) {
    CefQuitMessageLoop();
    return;
  }

  exit_ = true;
}


void Handler::GetSourceCodeDelayed_(CefRefPtr<CefBrowser> browser) {
  LOG(INFO) << "getting source code";

  CefRefPtr<CefStringVisitor> visitor = common::StringVisitorFromCallback(
      base::Bind(&Handler::VisitSourceCode_, this, browser));
  browser->GetMainFrame()->GetSource(visitor);
}


void Handler::VisitSourceCode_(CefRefPtr<CefBrowser> browser,
                               const CefString& source) {
  // Reduce the count of pending requests. If we reach zero and we're exiting
  // the app, quit the message loop too
  base::AutoLock lock_scope(pending_lock_);
  pending_--;

  // Write response
  seo::Response response;
  response.set_status(Response_Status_OK);
  response.set_source_code(source.ToString());

  if (!common::WriteDelimitedTo(response, &output_stream_)) {
    LOG(FATAL) << "cannot write response message to stdout";
  }

  // Flush stdout or otherwise some of the content may not appear in the output
  output_stream_.Flush();

  VLOG(1) << "source obtained. closing browser...";

  // browser->GetHost()->CloseBrowser(false /* force_close */);

  VLOG(2) << "browser closed; pending requests -> " << pending_ <<
      "; exit -> " << exit_;
  if (pending_ == 0 && exit_) {
    CefQuitMessageLoop();
  }
}


}  // namespace seo
