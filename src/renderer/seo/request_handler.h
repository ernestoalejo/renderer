// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_REQUEST_HANDLER_H_
#define RENDERER_REQUEST_HANDLER_H_

#include "include/cef_request_handler.h"


namespace seo {


class RequestHandler : public CefRequestHandler {
 public:
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

  void Init();

 private:
  std::vector<std::string> blacklisted_domains_;

  IMPLEMENT_REFCOUNTING(RequestHandler);
};


}  // namespace seo


#endif  // RENDERER_REQUEST_HANDLER_H_
