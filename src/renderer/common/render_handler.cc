// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/common/render_handler.h"


namespace common {


RenderHandler::RenderHandler(int width, int height)
: width_(width), height_(height) {
  // empty
}


bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
  rect.Set(0, 0, width_, height_);
  return true;
}


void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type, const RectList &dirtyRects,
                            const void *buffer, int width, int height) {
  // empty
}


}  // namespace common
