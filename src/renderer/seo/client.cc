// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/client.h"

#include "glog/logging.h"
#include "include/cef_app.h"

#include "base/util.h"

namespace seo {

Client::Client(uint64_t id, const std::string& url) {
  request_ = new Request(id, url);

  // display_handler_ = new DisplayHandler();
  // load_handler_ = new LoadHandler(request_);
  render_handler_ = new common::RenderHandler(1900, 800);
  // request_handler_ = new RequestHandler(request_);
  // life_span_handler_ = new LifeSpanHandler(request_);
}

void Client::Init() {
  REQUIRE_FILE_THREAD();

  // request_handler_->Init();
}

}  // namespace seo
