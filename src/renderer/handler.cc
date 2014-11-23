// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/handler.h"

#include <glog/logging.h>

#include "base/util.h"
#include "include/cef_app.h"


namespace {

Handler* g_instance = NULL;

}  // namespace


Handler::Handler(CefRefPtr<RenderHandler> render_handler,
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
    LOG(INFO) << "page loaded: " << browser->GetMainFrame()->GetURL().ToString();
    ExtractSourceCode(browser);
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

void Handler::ExtractSourceCode(CefRefPtr<CefBrowser> browser) {
  class Visitor : public CefStringVisitor {
  public:
    virtual void Visit(const CefString& source) OVERRIDE {
      LOG(INFO) << "source obtained";
      // LOG(INFO) << source.ToString();

      CefQuitMessageLoop();
    }

  private:
    IMPLEMENT_REFCOUNTING(Visitor);
  };

  browser->GetMainFrame()->GetSource(new Visitor());
}
