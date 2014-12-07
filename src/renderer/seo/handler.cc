// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/handler.h"

#include <iostream>

#include <glog/logging.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "base/util.h"
#include "include/cef_app.h"
#include "proto/seo/response.pb.h"
#include "renderer/common/protobufs.h"


namespace seo {

namespace {

Handler* g_instance = NULL;


class SourceCodeVisitor : public CefStringVisitor {
public:
  virtual void Visit(const CefString& source) OVERRIDE {
    // Write response
    seo::Response response;
    response.set_status(Response_Status_OK);
    response.set_source_code(source.ToString());

    google::protobuf::io::OstreamOutputStream outputStream(&std::cout);
    if (!common::WriteDelimitedTo(response, &outputStream)) {
      LOG(FATAL) << "cannot write response message to stdout";
    }

    LOG(INFO) << "source obtained";
  }

private:
  IMPLEMENT_REFCOUNTING(SourceCodeVisitor);
};


class SourceCodeDelayed : public CefTask {
public:
  SourceCodeDelayed(CefRefPtr<CefBrowser> browser)
  : browser_(browser) {
    // empty
  }

  virtual void Execute() OVERRIDE {
    LOG(INFO) << "getting source code";

    browser_->GetMainFrame()->GetSource(new SourceCodeVisitor());
  }

private:
  CefRefPtr<CefBrowser> browser_;

  IMPLEMENT_REFCOUNTING(SourceCodeDelayed);
};


}  // namespace


Handler::Handler(CefRefPtr<common::RenderHandler> render_handler,
                 CefRefPtr<RequestHandler> request_handler)
: render_handler_(render_handler), request_handler_(request_handler)
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
    CefPostDelayedTask(TID_UI, new SourceCodeDelayed(browser), 500);
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


}  // namespace seo
