// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef BASE_VISITORS_H_
#define BASE_VISITORS_H_

#include "include/base/cef_callback.h"
#include "include/cef_string_visitor.h"

namespace base {

CefRefPtr<CefStringVisitor> StringVisitorFromCallback(
  const base::Callback<void(const CefString& source)>& callback);

}  // namespace base

#endif  // BASE_VISITORS_H_
