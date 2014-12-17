// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_CLIENT_H_
#define RENDERER_SEO_CLIENT_H_

#include <string>

#include "glog/logging.h"
#include "include/base/cef_lock.h"
#include "include/cef_client.h"

#include "proto/seo/response.pb.h"
#include "renderer/common/render_handler.h"
#include "renderer/seo/display_handler.h"
#include "renderer/seo/life_span_handler.h"
#include "renderer/seo/load_handler.h"
#include "renderer/seo/request.h"
#include "renderer/seo/request_handler.h"

namespace seo {

class Client : public CefClient {
 public:
  Client(uint64_t id, const std::string& url);

  // CefClient methods
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return load_handler_;
  }
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
    return render_handler_;
  }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
    return request_handler_;
  }
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return display_handler_;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return life_span_handler_;
  }

  void Init();

 private:
  CefRefPtr<Request> request_;
  CefRefPtr<common::RenderHandler> render_handler_;
  CefRefPtr<RequestHandler> request_handler_;
  CefRefPtr<DisplayHandler> display_handler_;
  CefRefPtr<LoadHandler> load_handler_;
  CefRefPtr<LifeSpanHandler> life_span_handler_;

  IMPLEMENT_REFCOUNTING(Client);
};

}  // namespace seo

#endif  // RENDERER_SEO_CLIENT_H_
