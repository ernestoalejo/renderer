// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/app.h"

#include <glog/logging.h>

#include "base/util.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "renderer/seo/handler.h"


namespace seo {


void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  render_handler_ = new common::RenderHandler(1900, 800);
  request_handler_ = new RequestHandler;
  request_handler_->Initialize();

  CefRefPtr<Handler> handler(new Handler(render_handler_, request_handler_));
  CefBrowserSettings browser_settings;

  CefWindowInfo window_info;
  window_info.SetAsOffScreen(NULL);
  window_info.SetTransparentPainting(true);

  CefBrowserHost::CreateBrowser(window_info, handler.get(), "http://www.google.com/",
      browser_settings, NULL);
}


}  // namespace seo
