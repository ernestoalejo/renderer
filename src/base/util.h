// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef BASE_UTIL_H_
#define BASE_UTIL_H_


#include <cassert>

#include "include/cef_task.h"


#ifndef NDEBUG
  #define ASSERT(condition) if (!(condition)) { assert(false); }
#else
  #define ASSERT(condition) ((void) 0)
#endif

#define REQUIRE_UI_THREAD()   ASSERT(CefCurrentlyOn(TID_UI));
#define REQUIRE_IO_THREAD()   ASSERT(CefCurrentlyOn(TID_IO));
#define REQUIRE_FILE_THREAD() ASSERT(CefCurrentlyOn(TID_FILE));


#endif  // BASE_UTIL_H_
