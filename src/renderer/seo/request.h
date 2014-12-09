// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_REQUEST_H_
#define RENDERER_SEO_REQUEST_H_

#include <string>

#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"

#include "proto/seo/response.pb.h"


namespace seo {


class Request {
 public:
  Request(uint64_t id, const std::string& url);

  uint64_t id() {
    return id_;
  }

  std::string url() {
    return url_;
  }

  bool failed() {
    return failed_;
  }

  bool closing() {
    return closing_;
  }

  CefRefPtr<CefBrowser> browser() {
    return browser_;
  }

  void set_browser(CefRefPtr<CefBrowser> browser) {
    browser_ = browser;
  }

  void EmitError(proto::seo::Response_Status status);
  void EmitSourceCode(const CefString& source_code);
  void EmitRedirection(const std::string& url);

 private:
  uint64_t id_;
  std::string url_;

  google::protobuf::io::FileOutputStream output_stream_;

  bool failed_, closing_;

  CefRefPtr<CefBrowser> browser_;

  void Write_(const proto::seo::Response& response);

  void CloseBrowser_();
  void CloseBrowserUIThread_();

  IMPLEMENT_REFCOUNTING(Request);
};


}  // namespace seo


#endif  // RENDERER_SEO_REQUEST_H_
