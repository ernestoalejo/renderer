// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_RESOURCE_HANDLER_H_
#define RENDERER_SEO_RESOURCE_HANDLER_H_

#include "include/base/cef_lock.h"
#include "include/cef_resource_handler.h"
#include "include/cef_urlrequest.h"

#include "renderer/seo/request.h"

namespace seo {

class ResourceHandler : public CefResourceHandler, public CefURLRequestClient {
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

  // CefURLRequestClient methods
  virtual bool GetAuthCredentials(bool is_proxy, const CefString& host,
                                  int port, const CefString& realm,
                                  const CefString& scheme,
                                  CefRefPtr<CefAuthCallback> callback) OVERRIDE;
  virtual void OnDownloadData(CefRefPtr<CefURLRequest> request,
                              const void* data, size_t data_length) OVERRIDE;
  virtual void OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                 uint64 current, uint64 total) OVERRIDE;
  virtual void OnRequestComplete(CefRefPtr<CefURLRequest> request) OVERRIDE;
  virtual void OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                uint64 current, uint64 total) OVERRIDE;

 private:
  CefRefPtr<Request> request_;

  base::Lock lock_;

  std::vector<unsigned char> buffer_;
  int buffer_read_;
  bool complete_;

  CefRefPtr<CefCallback> read_response_callback_;

  void ProcessRequestUIThread_(CefRefPtr<CefRequest> request,
                               CefRefPtr<CefCallback> callback);
  void CallReadResponseCallback_();

  IMPLEMENT_REFCOUNTING(ResourceHandler);
};

}  // namespace seo

#endif  // RENDERER_SEO_RESOURCE_HANDLER_H_
