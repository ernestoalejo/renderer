// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_DISPLAY_HANDLER_H_
#define RENDERER_SEO_DISPLAY_HANDLER_H_

#include <string>
#include <vector>

#include "include/cef_display_handler.h"


namespace seo {


class DisplayHandler : public CefDisplayHandler {
 public:
  // CefDisplayHandler methods
  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& url) OVERRIDE;

 private:
  IMPLEMENT_REFCOUNTING(DisplayHandler);
};


}  // namespace seo


#endif  // RENDERER_SEO_DISPLAY_HANDLER_H_
