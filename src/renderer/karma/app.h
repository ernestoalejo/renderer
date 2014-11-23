// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_KARMA_APP_H_
#define RENDERER_KARMA_APP_H_

#include "include/cef_app.h"

#include "renderer/karma/render_handler.h"
#include "renderer/karma/request_handler.h"


namespace karma {


class App : public CefApp, public CefBrowserProcessHandler {
public:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
    return this;
  }

  virtual void OnContextInitialized() OVERRIDE;

private:
  CefRefPtr<RenderHandler> render_handler_;
  CefRefPtr<RequestHandler> request_handler_;

  IMPLEMENT_REFCOUNTING(App);
};


}  // namespace karma


#endif  // RENDERER_KARMA_APP_H_

