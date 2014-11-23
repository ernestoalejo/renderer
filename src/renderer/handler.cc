// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/handler.h"

#include <iostream>
#include <cstdlib>

#include "base/util.h"


namespace {

Handler* g_instance = NULL;

}  // namespace


Handler::Handler(CefRefPtr<RenderHandler> render_handler) {
  ASSERT(!g_instance);
  g_instance = this;
  render_handler_ = render_handler;
}

Handler::~Handler() {
  g_instance = NULL;
}

Handler* Handler::GetInstance() {
  return g_instance;
}

void Handler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  // Save a reference to the created browser
  browser_list_.push_back(browser);
}

void Handler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  REQUIRE_UI_THREAD();

  // Remove our reference to the browser
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }
}

void Handler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                   bool isLoading, bool canGoBack,
                                   bool canGoForward) {
  REQUIRE_UI_THREAD();

  if (!isLoading) {
    std::cout << "not loading" << std::endl;
  }
}

void Handler::OnLoadError(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          ErrorCode errorCode, const CefString& errorText,
                          const CefString& failedUrl) {
  REQUIRE_UI_THREAD();

  std::cout << "error!" << std::string(errorText) << " in " << std::string(failedUrl) << std::endl;
}
