// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_HANDLER_H_
#define RENDERER_HANDLER_H_

#include "include/cef_client.h"

#include <list>

#include "renderer/render_handler.h"
#include "renderer/request_handler.h"


class Handler : public CefClient,
                public CefLifeSpanHandler,
                public CefLoadHandler {
public:
  Handler(CefRefPtr<RenderHandler> render_handler,
          CefRefPtr<RequestHandler> request_handler);
  ~Handler();

  // Provide access to the single global instance of this object.
  static Handler* GetInstance();

  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
    return render_handler_;
  }

  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE {
    return request_handler_;
  }

  // CefLifeSpanHandler methods
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  // CefLoadHandler methods
  virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                    bool isLoading, bool canGoBack,
                                    bool canGoForward) OVERRIDE;
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

private:
  typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
  BrowserList browser_list_;

  CefRefPtr<CefRenderHandler> render_handler_;
  CefRefPtr<CefRequestHandler> request_handler_;

  void ExtractSourceCode(CefRefPtr<CefBrowser> browser);

  IMPLEMENT_REFCOUNTING(Handler);
};

#endif  // RENDERER_HANDLER_H_
