// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/app.h"


int main(int argc, char* argv[]) {
  CefMainArgs main_args(argc, argv);

  CefRefPtr<CefApp> app(GetCurrentApp());

  // Execute the subprocess logic and wait until it finishes
  return CefExecuteProcess(main_args, app.get(), NULL);
}
