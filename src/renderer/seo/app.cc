// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/app.h"

#include <iostream>
#include <thread>

#include <glog/logging.h>

#include "base/util.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"


namespace seo {


void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  CefRefPtr<common::RenderHandler> render_handler = new common::RenderHandler(1900, 800);
  CefRefPtr<RequestHandler> request_handler = new RequestHandler;
  request_handler->Initialize();

  handler_ = new Handler(render_handler, request_handler);

  // CefBrowserSettings browser_settings;

  // CefWindowInfo window_info;
  // window_info.SetAsOffScreen(NULL);
  // window_info.SetTransparentPainting(true);

  std::thread thread(std::bind(&App::ReadRequests, this));
  thread.detach();

  // CefBrowserHost::CreateBrowser(window_info, handler.get(), url.c_str(),
      // browser_settings, NULL);
}


void App::ReadRequests() {
  LOG(INFO) << "running requests reader";

  std::string size;
  std::cin >> size;

  LOG(INFO) << "read size: " << size;
}


}  // namespace seo
