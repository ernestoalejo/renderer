// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/karma/client.h"

#include "glog/logging.h"
#include "include/cef_app.h"

#include "base/util.h"

namespace karma {

Client::Client() {
  render_handler_ = new common::RenderHandler(1900, 800);
}

void Client::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                   bool isLoading, bool canGoBack,
                                   bool canGoForward) {
  REQUIRE_UI_THREAD();

  if (!isLoading) {
    LOG(INFO) << "page loaded: " <<
        browser->GetMainFrame()->GetURL().ToString();
  }
}

void Client::OnLoadError(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          ErrorCode errorCode, const CefString& errorText,
                          const CefString& failedUrl) {
  REQUIRE_UI_THREAD();

  if (errorCode == ERR_CONNECTION_REFUSED) {
    LOG(FATAL) << "cannot connect to " << failedUrl.ToString();
  }

  LOG(FATAL) << "error loading (" << errorCode << "): " <<
      errorText.ToString();
}

}  // namespace karma
