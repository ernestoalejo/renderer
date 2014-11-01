// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/handler.h"

#include "base/util.h"


namespace {

Handler* g_instance = NULL;

}  // namespace


Handler::Handler() {
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

// #include <sstream>
// #include <string>

// #include "include/cef_app.h"



// void Handler::OnLoadError(CefRefPtr<CefBrowser> browser,
//                                 CefRefPtr<CefFrame> frame,
//                                 ErrorCode errorCode,
//                                 const CefString& errorText,
//                                 const CefString& failedUrl) {
//   REQUIRE_UI_THREAD();

//   // Don't display an error for downloaded files.
//   if (errorCode == ERR_ABORTED)
//     return;

//   // Display a load error message.
//   std::stringstream ss;
//   ss << "<html><body bgcolor=\"white\">"
//         "<h2>Failed to load URL " << std::string(failedUrl) <<
//         " with error " << std::string(errorText) << " (" << errorCode <<
//         ").</h2></body></html>";
//   frame->LoadString(ss.str(), failedUrl);
// }

// void Handler::CloseAllBrowsers(bool force_close) {
//   if (!CefCurrentlyOn(TID_UI)) {
//     // Execute on the UI thread.
//     CefPostTask(TID_UI,
//         NewCefRunnableMethod(this, &Handler::CloseAllBrowsers,
//                              force_close));
//     return;
//   }

//   if (browser_list_.empty())
//     return;

//   BrowserList::const_iterator it = browser_list_.begin();
//   for (; it != browser_list_.end(); ++it)
//     (*it)->GetHost()->CloseBrowser(force_close);
// }
