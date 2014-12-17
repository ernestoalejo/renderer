// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_KARMA_CLIENT_H_
#define RENDERER_KARMA_CLIENT_H_

#include "include/cef_client.h"

#include "renderer/common/render_handler.h"

namespace karma {

class Client : public CefClient,
                public CefLoadHandler {
 public:
  Client();

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }

  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE {
    return render_handler_;
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

  IMPLEMENT_REFCOUNTING(Client);
};

}  // namespace karma

#endif  // RENDERER_KARMA_CLIENT_H_
