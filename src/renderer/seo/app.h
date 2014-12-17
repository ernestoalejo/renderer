// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_APP_H_
#define RENDERER_SEO_APP_H_

#include "include/cef_app.h"

namespace seo {

class App : public CefApp, public CefBrowserProcessHandler {
 public:
  virtual CefRefPtr<CefBrowserProcessHandler>
      GetBrowserProcessHandler() OVERRIDE {
    return this;
  }

  // CefBrowserProcessHandler methods
  virtual void OnContextInitialized() OVERRIDE;

 private:
  void ReadRequests_();

  IMPLEMENT_REFCOUNTING(App);
};

}  // namespace seo

#endif  // RENDERER_SEO_APP_H_

