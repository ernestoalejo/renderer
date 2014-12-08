// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_HANDLER_H_
#define RENDERER_HANDLER_H_

#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "include/cef_client.h"
#include "include/base/cef_lock.h"

#include "proto/seo/response.pb.h"
#include "renderer/common/render_handler.h"
#include "renderer/seo/request_handler.h"


namespace seo {


class Handler : public CefClient,
                public CefLoadHandler {
public:
  Handler(uint64_t id);
  ~Handler();

  // Provide access to the single global instance of this object.
  static Handler* GetInstance();

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
    return render_handler_;
  }

  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
    return request_handler_;
  }

  // CefLoadHandler methods
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading, bool canGoBack,
                                    bool canGoForward) OVERRIDE;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

private:
  CefRefPtr<CefRenderHandler> render_handler_;
  CefRefPtr<CefRequestHandler> request_handler_;

  google::protobuf::io::FileOutputStream output_stream_;

  uint64_t id_;

  // Signals an error when loading the page, so the source won't be sent back
  // to the requester
  bool loading_error_;

  void GetSourceCodeDelayed_(CefRefPtr<CefBrowser> browser);
  void VisitSourceCode_(CefRefPtr<CefBrowser> browser, const CefString& source);

  void LoadingError_(CefRefPtr<CefBrowser> browser, Response_Status status);

  void CloseBrowser_(CefRefPtr<CefBrowser> browser);
  void CloseBrowserUIThread_(CefRefPtr<CefBrowser> browser);

  IMPLEMENT_REFCOUNTING(Handler);
};


void CountNewHandler();
void ExitAllHandlers();


}  // namespace seo


#endif  // RENDERER_HANDLER_H_
