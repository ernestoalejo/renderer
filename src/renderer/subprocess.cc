// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/app.h"


int main(int argc, char* argv[]) {
  CefMainArgs main_args(argc, argv);

  CefRefPtr<App> app(new App);

  // Execute the subprocess logic and wait until it finishes
  return CefExecuteProcess(main_args, app.get(), NULL);
}
