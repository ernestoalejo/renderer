// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_COMMON_VISITORS_H_
#define RENDERER_COMMON_VISITORS_H_

#include "include/base/cef_callback.h"
#include "include/cef_string_visitor.h"


namespace common {


CefRefPtr<CefStringVisitor> StringVisitorFromCallback(
  const base::Callback<void(const CefString& source)>& callback);


}  // namespace common


#endif  // RENDERER_COMMON_VISITORS_H_
