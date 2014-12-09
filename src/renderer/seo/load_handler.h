// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_LOAD_HANDLER_H_
#define RENDERER_SEO_LOAD_HANDLER_H_

#include "include/cef_load_handler.h"

#include "renderer/seo/request.h"


namespace seo {


class LoadHandler : public CefLoadHandler {
 public:
  explicit LoadHandler(Request* request_);

  // CefLoadHandler methods
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading, bool canGoBack,
                                    bool canGoForward) OVERRIDE;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

 private:
  Request* request_;

  void GetSourceCodeDelayed_();
  void VisitSourceCode_(const CefString& source);

  IMPLEMENT_REFCOUNTING(LoadHandler);
};


}  // namespace seo


#endif  // RENDERER_SEO_LOAD_HANDLER_H_
