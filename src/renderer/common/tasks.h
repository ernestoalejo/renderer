// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_COMMON_TASKS_H_
#define RENDERER_COMMON_TASKS_H_

#include "include/base/cef_callback.h"
#include "include/cef_task.h"


namespace common {


CefRefPtr<CefTask> TaskFromCallback(const base::Callback<void()>& callback);


}  // namespace common


#endif  // RENDERER_COMMON_TASKS_H_
