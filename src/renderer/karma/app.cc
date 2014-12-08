// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/karma/app.h"

#include <glog/logging.h>

#include "base/util.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "renderer/karma/handler.h"


DEFINE_string(url, "", "url to load");


namespace karma {


void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  render_handler_ = new common::RenderHandler(1900, 800);

  CefRefPtr<Handler> handler(new Handler(render_handler_));
  
  CefBrowserSettings browser_settings;
  CefWindowInfo window_info;

  std::string url(FLAGS_url);
  if (url.empty()) {
    LOG(FATAL) << "the url flag cannot be empty";
  }

  LOG(INFO) << "opening: " << url;
  CefBrowserHost::CreateBrowser(window_info, handler.get(), url.c_str(),
      browser_settings, NULL);
}


}  // namespace karma
