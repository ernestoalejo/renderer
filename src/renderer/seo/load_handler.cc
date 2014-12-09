// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/load_handler.h"

#include "glog/logging.h"
#include "include/base/cef_bind.h"

#include "base/util.h"
#include "renderer/common/tasks.h"
#include "renderer/common/visitors.h"


namespace seo {


LoadHandler::LoadHandler(Request* request)
: request_(request) {
  // empty
}


void LoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                   bool isLoading, bool canGoBack,
                                   bool canGoForward) {
  REQUIRE_UI_THREAD();

  if (request_->failed() || request_->closing()) {
    return;
  }

  if (!isLoading) {
    LOG(INFO) << "page loaded: " << request_->url();

    CefRefPtr<CefTask> task = common::TaskFromCallback(
        base::Bind(&LoadHandler::GetSourceCodeDelayed_, this));
    CefPostDelayedTask(TID_UI, task, 500);
  }
}


void LoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          ErrorCode errorCode, const CefString& errorText,
                          const CefString& failedUrl) {
  REQUIRE_UI_THREAD();

  if (request_->failed() || request_->closing()) {
    return;
  }

  switch (static_cast<int>(errorCode)) {
  case ERR_CONNECTION_REFUSED:
    LoadingError_( proto::seo::Response_Status_CONNECTION_REFUSED);
    break;

  case ERR_NAME_NOT_RESOLVED:
    LoadingError_( proto::seo::Response_Status_NAME_NOT_RESOLVED);
    break;

  case ERR_ABORTED:
    // Aborted means probably an iframe with a origin we blacklisted or
    // something like that (as there is no UI where the user can "abort" the
    // request). Ignore the error directly.
    break;

  case -137:  // ERR_NAME_RESOLUTION_FAILED not present
    LoadingError_( proto::seo::Response_Status_NAME_RESOLUTION_FAILED);
    break;

  default:
    LOG(FATAL) << "error loading (" << errorCode << "): " <<
        errorText.ToString();
  }
}


void LoadHandler::GetSourceCodeDelayed_() {
  VLOG(1) << "getting source code...";

  CefRefPtr<CefStringVisitor> visitor = common::StringVisitorFromCallback(
      base::Bind(&LoadHandler::VisitSourceCode_, this));
  request_->browser()->GetMainFrame()->GetSource(visitor);
}


void LoadHandler::VisitSourceCode_(const CefString& source) {
  if (request_->closing() || request_->failed()) {
    return;
  }

  LOG(INFO) << "source obtained: " << request_->url();

  request_->EmitSourceCode(source);
}


void LoadHandler::LoadingError_(proto::seo::Response_Status status) {
  // Signal the loading error internally and by stderr
  LOG(ERROR) << "load error: " << status;

  request_->EmitError(status);
}


}  // namespace seo
