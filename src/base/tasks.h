// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef COMMON_TASKS_H_
#define COMMON_TASKS_H_

#include "include/base/cef_callback.h"
#include "include/cef_task.h"

namespace base {

CefRefPtr<CefTask> TaskFromCallback(const base::Callback<void()>& callback);

}  // namespace base

#endif  // COMMON_TASKS_H_
