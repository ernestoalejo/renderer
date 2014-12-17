// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/karma/app.h"

#include <string>

#include "glog/logging.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"

#include "base/util.h"
#include "renderer/karma/client.h"

DEFINE_string(url, "", "url to load");

namespace karma {

void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  std::string url(FLAGS_url);
  if (url.empty()) {
    LOG(FATAL) << "the url flag cannot be empty";
  }

  CefBrowserSettings browser_settings;
  
  CefWindowInfo window_info;
  window_info.windowless_rendering_enabled = true;

  LOG(INFO) << "opening: " << url;
  CefRefPtr<Client> client(new Client());
  CefBrowserHost::CreateBrowser(window_info, client.get(), url.c_str(),
      browser_settings, NULL);
}

}  // namespace karma
