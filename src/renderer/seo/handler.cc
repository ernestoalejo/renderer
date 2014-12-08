// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/handler.h"

#include <iostream>

#include <glog/logging.h>

#include "include/base/cef_bind.h"

#include "base/util.h"
#include "include/cef_app.h"
#include "renderer/common/protobufs.h"
#include "renderer/common/visitors.h"
#include "renderer/common/tasks.h"


namespace seo {

namespace {

Handler* g_instance = NULL;

bool g_exit_all_handlers = false;

int g_pending_handlers = 0;
base::Lock g_pending_handlers_lock;

}  // namespace


Handler::Handler(CefRefPtr<common::RenderHandler> render_handler,
                 CefRefPtr<RequestHandler> request_handler, uint64_t id)
: render_handler_(render_handler), request_handler_(request_handler),
  output_stream_(STDOUT_FILENO), id_(id), loading_error_(false)
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

  if (loading_error_) {
    return;
  }

  if (!isLoading) {
    LOG(INFO) << "page loaded: " <<
        browser->GetMainFrame()->GetURL().ToString();

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

  switch (int(errorCode)) {
  case ERR_CONNECTION_REFUSED:
    LoadingError_(browser, Response_Status_CONNECTION_REFUSED);
    break;

  case ERR_NAME_NOT_RESOLVED:
    LoadingError_(browser, Response_Status_NAME_NOT_RESOLVED);
    break;

  case ERR_ABORTED:
    // Aborted means probably an iframe with a origin we blacklisted or
    // something like that (as there is no UI where the user can "abort" the
    // request). Ignore the error directly.
    break;

  case -137:  // ERR_NAME_RESOLUTION_FAILED not present
    LoadingError_(browser, Response_Status_NAME_RESOLUTION_FAILED);
    break;

  default:
    LOG(FATAL) << "error loading (" << errorCode << "): " << errorText.ToString();
  }
}


void Handler::GetSourceCodeDelayed_(CefRefPtr<CefBrowser> browser) {
  VLOG(1) << "getting source code...";

  CefRefPtr<CefStringVisitor> visitor = common::StringVisitorFromCallback(
      base::Bind(&Handler::VisitSourceCode_, this, browser));
  browser->GetMainFrame()->GetSource(visitor);
}


void Handler::VisitSourceCode_(CefRefPtr<CefBrowser> browser,
                               const CefString& source) {
  // Reduce the count of pending requests. If we reach zero and we're exiting
  // the app, quit the message loop too
  base::AutoLock lock_scope(g_pending_handlers_lock);
  g_pending_handlers--;

  // Write response
  seo::Response response;
  response.set_status(Response_Status_OK);
  response.set_source_code(source.ToString());
  response.set_id(id_);

  if (!common::WriteDelimitedTo(response, &output_stream_)) {
    LOG(FATAL) << "cannot write response message to stdout";
  }

  // Flush stdout or otherwise some of the content may not appear in the output
  output_stream_.Flush();

  LOG(INFO) << "source obtained: " <<
      browser->GetMainFrame()->GetURL().ToString();

  CloseBrowser_(browser);
}


void Handler::LoadingError_(CefRefPtr<CefBrowser> browser,
                         Response_Status status) {
  // Reduce the count of pending requests. If we reach zero and we're exiting
  // the app, quit the message loop too
  base::AutoLock lock_scope(g_pending_handlers_lock);
  g_pending_handlers--;

  // Write response
  seo::Response response;
  response.set_status(status);
  response.set_id(id_);

  if (!common::WriteDelimitedTo(response, &output_stream_)) {
    LOG(FATAL) << "cannot write response message to stdout";
  }

  // Flush stdout or otherwise some of the content may not appear in the output
  output_stream_.Flush();

  // Signal the loading error internally and by stderr
  LOG(ERROR) << "load error: " << status;
  loading_error_ = true;

  // Close the browser process
  CloseBrowser_(browser);
}


void Handler::CloseBrowser_(CefRefPtr<CefBrowser> browser) {
  // Send the close order to the browser window in the correct thread
  auto callback = base::Bind(&Handler::CloseBrowserUIThread_, this, browser);
  CefPostTask(TID_UI, common::TaskFromCallback(callback));

  VLOG(2) << "browser closed; pending requests -> " << g_pending_handlers <<
      "; exit -> " << g_exit_all_handlers;
  if (g_pending_handlers == 0 && g_exit_all_handlers) {
    CefQuitMessageLoop();
  }
}


void Handler::CloseBrowserUIThread_(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  browser->GetHost()->CloseBrowser(true);
}


void CountNewHandler() {
  base::AutoLock lock_scope(g_pending_handlers_lock);
  g_pending_handlers++;
}


void ExitAllHandlers() {
  base::AutoLock lock_scope(g_pending_handlers_lock);

  VLOG(2) << "exit all handlers; pending requests -> " << g_pending_handlers;
  if (g_pending_handlers == 0) {
    CefPostTask(TID_UI,
        common::TaskFromCallback(base::Bind(&CefQuitMessageLoop)));
    return;
  }

  g_exit_all_handlers = true;
}


}  // namespace seo
