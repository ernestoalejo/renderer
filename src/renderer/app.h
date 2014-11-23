// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_APP_H_
#define RENDERER_APP_H_

#include "include/cef_app.h"

#include "renderer/render_handler.h"


class App : public CefApp, public CefBrowserProcessHandler {
public:
  App();

  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE {
    return this;
  }

  virtual void OnContextInitialized() OVERRIDE;

private:
  CefRefPtr<RenderHandler> render_handler_;

  IMPLEMENT_REFCOUNTING(App);
};


#endif  // RENDERER_APP_H_

