// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_HANDLER_H_
#define RENDERER_HANDLER_H_

#include "include/cef_client.h"
#include "include/base/cef_lock.h"

#include "renderer/common/render_handler.h"
#include "renderer/seo/request_handler.h"


namespace seo {


class Handler : public CefClient,
                public CefLoadHandler {
public:
  Handler(CefRefPtr<common::RenderHandler> render_handler,
          CefRefPtr<seo::RequestHandler> request_handler);
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

  void CountPendingRequest();
  void Exit();

private:
  CefRefPtr<CefRenderHandler> render_handler_;
  CefRefPtr<CefRequestHandler> request_handler_;
  
  bool exit_;

  int pending_;
  base::Lock pending_lock_;

  void GetSourceCodeDelayed_(CefRefPtr<CefBrowser> browser);
  void VisitSourceCode_(const CefString& source);

  IMPLEMENT_REFCOUNTING(Handler);
};


}  // namespace seo


#endif  // RENDERER_HANDLER_H_
