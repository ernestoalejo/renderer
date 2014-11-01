// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_HANDLER_H_
#define RENDERER_HANDLER_H_

#include "include/cef_client.h"

#include <list>


class Handler : public CefClient, public CefLifeSpanHandler, public CefLoadHandler {
public:
  Handler();
  ~Handler();

  // Provide access to the single global instance of this object.
  static Handler* GetInstance();

  // virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
  //   return this;
  // }

  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }

  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
    return this;
  }

  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

private:
  IMPLEMENT_REFCOUNTING(Handler);

  typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
  BrowserList browser_list_;
};

#endif  // RENDERER_HANDLER_H_



//   // CefDisplayHandler methods:
//   virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
//                              const CefString& title) OVERRIDE;

//   // CefLoadHandler methods:
//   virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
//                            CefRefPtr<CefFrame> frame,
//                            ErrorCode errorCode,
//                            const CefString& errorText,
//                            const CefString& failedUrl) OVERRIDE;

//   // Request that all existing browser windows close.
//   void CloseAllBrowsers(bool force_close);

//   bool IsClosing() const { return is_closing_; }

//  private:
//   // List of existing browser windows. Only accessed on the CEF UI thread.
//   typedef std::list<CefRefPtr<CefBrowser> > BrowserList;
//   BrowserList browser_list_;

//   bool is_closing_;
// };
