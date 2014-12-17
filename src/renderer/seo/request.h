// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_REQUEST_H_
#define RENDERER_SEO_REQUEST_H_

#include <string>

#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "include/base/cef_lock.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"

#include "proto/seo/response.pb.h"

namespace seo {

class Request {
 public:
  Request(uint64_t id, const std::string& url);

  uint64_t id();

  std::string url();

  bool failed();
  bool closing();

  CefRefPtr<CefBrowser> browser();
  void set_browser(CefRefPtr<CefBrowser> browser);

  int pending_requests();
  void IncrementPendingRequests();
  void DecrementPendingRequests();

  void EmitError(proto::seo::Response_Status status);
  void EmitSourceCode(const CefString& source_code);
  void EmitRedirection(const std::string& url);

 private:
  base::Lock lock_;

  uint64_t id_;
  std::string url_;

  google::protobuf::io::FileOutputStream output_stream_;

  bool failed_, closing_;

  CefRefPtr<CefBrowser> browser_;

  int pending_requests_;

  void Write_(const proto::seo::Response& response);

  void CloseBrowser_();
  void CloseBrowserUIThread_();

  IMPLEMENT_REFCOUNTING(Request);
};

}  // namespace seo

#endif  // RENDERER_SEO_REQUEST_H_
