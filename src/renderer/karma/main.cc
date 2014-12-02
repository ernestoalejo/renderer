// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <gflags/gflags.h>

#include "renderer/common/initialize.h"
#include "renderer/karma/app.h"


int main(int argc, char* argv[]) {
  InitializeBeforeApp(argv);

  karma::App* app = new karma::App;

  int result = InitializeAfterApp(argc, argv, CefRefPtr<CefApp>(app));

  return result;
}
