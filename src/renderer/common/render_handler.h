// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_COMMON_RENDER_HANDLER_H_
#define RENDERER_COMMON_RENDER_HANDLER_H_

#include "include/cef_render_handler.h"


namespace common {


class RenderHandler : public CefRenderHandler {
 public:
  RenderHandler(int width, int height);

  // CefRenderHandler methods
  virtual bool GetViewRect(CefRefPtr<CefBrowser> browser,
                           CefRect& rect) OVERRIDE;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
               const RectList &dirtyRects, const void *buffer, int width,
               int height) OVERRIDE;

 private:
  int width_, height_;

  IMPLEMENT_REFCOUNTING(RenderHandler);
};


}  // namespace common


#endif  // RENDERER_COMMON_RENDER_HANDLER_H_
