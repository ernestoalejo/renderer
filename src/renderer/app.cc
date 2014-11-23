// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/app.h"

#include <string>

#include "base/util.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "renderer/handler.h"


void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  render_handler_ = new RenderHandler(1900, 800);
  request_handler_ = new RequestHandler;

  CefRefPtr<Handler> handler(new Handler(render_handler_, request_handler_));
  CefBrowserSettings browser_settings;

  CefWindowInfo window_info;
  window_info.SetAsOffScreen(NULL);
  window_info.SetTransparentPainting(true);

  CefBrowserHost::CreateBrowser(window_info, handler.get(),
      "http://www.laovejaverde.es/", browser_settings,
      NULL);
}
