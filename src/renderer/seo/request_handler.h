// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_REQUEST_HANDLER_H_
#define RENDERER_SEO_REQUEST_HANDLER_H_

#include <string>
#include <vector>

#include "include/cef_request_handler.h"

#include "renderer/seo/request.h"
#include "renderer/seo/resource_handler.h"

namespace seo {

class RequestHandler : public CefRequestHandler {
 public:
  explicit RequestHandler(CefRefPtr<Request> request);

  // CefRequestHandler methods
  virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                  const CefString& url,
                                  const CefString& policy_url,
                                  CefRefPtr<CefWebPluginInfo> info) OVERRIDE;
  virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request);
  virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  const CefString& old_url,
                                  CefString& new_url) OVERRIDE;
  virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
      CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request);

  void Init();

 private:
  std::vector<std::string> blacklisted_domains_;
  
  CefRefPtr<Request> request_;

  CefRefPtr<CefURLRequest> url_request_;

  IMPLEMENT_REFCOUNTING(RequestHandler);
};

}  // namespace seo

#endif  // RENDERER_SEO_REQUEST_HANDLER_H_
