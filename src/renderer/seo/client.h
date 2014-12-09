// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_CLIENT_H_
#define RENDERER_SEO_CLIENT_H_

#include "glog/logging.h"
#include "include/base/cef_lock.h"
#include "include/cef_client.h"

#include "proto/seo/response.pb.h"
#include "renderer/common/render_handler.h"
#include "renderer/seo/display_handler.h"
#include "renderer/seo/emitter.h"
#include "renderer/seo/request_handler.h"


namespace seo {


class Client : public CefClient,
                public CefLoadHandler {
 public:
  explicit Client(uint64_t id);

  // CefClient methods
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
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

  // CefLoadHandler methods
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading, bool canGoBack,
                                    bool canGoForward) OVERRIDE;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

  void Init();

 private:
  CefRefPtr<common::RenderHandler> render_handler_;
  CefRefPtr<RequestHandler> request_handler_;
  CefRefPtr<DisplayHandler> display_handler_;

  // Signals an error when loading the page, so the source won't be sent back
  // to the requester
  bool loading_error_;

  uint64_t id_;

  Emitter* emitter_;

  void GetSourceCodeDelayed_(CefRefPtr<CefBrowser> browser);
  void VisitSourceCode_(CefRefPtr<CefBrowser> browser, const CefString& source);

  void LoadingError_(CefRefPtr<CefBrowser> browser, Response_Status status);

  void CloseBrowser_(CefRefPtr<CefBrowser> browser);
  void CloseBrowserUIThread_(CefRefPtr<CefBrowser> browser);

  IMPLEMENT_REFCOUNTING(Client);
};


void CountNewHandler();
void ExitAllHandlers();


}  // namespace seo


#endif  // RENDERER_SEO_CLIENT_H_
