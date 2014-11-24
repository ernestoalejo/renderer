// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_COMMON_INITIALIZE_H_
#define RENDERER_COMMON_INITIALIZE_H_

#include "include/cef_app.h"


extern void InitializeBeforeApp(char* argv[]);
extern int InitializeAfterApp(int argc, char* argv[], CefRefPtr<CefApp> app);


#endif  // RENDERER_COMMON_INITIALIZE_H_
