// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/life_span_handler.h"

#include "glog/logging.h"


namespace seo {


LifeSpanHandler::LifeSpanHandler(Request* request)
: request_(request) {
  // empty
}


void LifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  request_->set_browser(browser);
}


}  // namespace seo
