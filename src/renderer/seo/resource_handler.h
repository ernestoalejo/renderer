// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_RESOURCE_HANDLER_H_
#define RENDERER_SEO_RESOURCE_HANDLER_H_

#include "include/base/cef_lock.h"
#include "include/cef_resource_handler.h"

#include "renderer/seo/request.h"

namespace seo {

class ResourceHandler : public CefResourceHandler {
 public:
  ResourceHandler(CefRefPtr<Request> request);

  // CefResourceHandler methods
  virtual void Cancel() OVERRIDE;
  virtual bool CanGetCookie(const CefCookie& cookie) OVERRIDE;
  virtual bool CanSetCookie(const CefCookie& cookie) OVERRIDE;
  virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
                                  int64& response_length,
                                  CefString& redirect_url) OVERRIDE;
  virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
                              CefRefPtr<CefCallback> callback) OVERRIDE;
  virtual bool ReadResponse(void* data_out, int bytes_to_read, int& bytes_read,
                            CefRefPtr<CefCallback> callback) OVERRIDE;

 private:
  CefRefPtr<Request> request_;

  base::Lock lock_;

  IMPLEMENT_REFCOUNTING(ResourceHandler);
};

}  // namespace seo

#endif  // RENDERER_SEO_RESOURCE_HANDLER_H_
