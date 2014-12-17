// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_LIFE_HANDLER_H_
#define RENDERER_SEO_LIFE_HANDLER_H_

#include "include/cef_life_span_handler.h"

#include "renderer/seo/request.h"

namespace seo {

class LifeSpanHandler : public CefLifeSpanHandler {
 public:
  explicit LifeSpanHandler(Request* request_);

  static int CountOpenBrowsers();
  static void NewBrowserCreated();

  // CefLifeSpanHandler methods
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

 private:
  Request* request_;

  IMPLEMENT_REFCOUNTING(LifeSpanHandler);
};

}  // namespace seo


#endif  // RENDERER_SEO_LIFE_HANDLER_H_
