// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <gflags/gflags.h>
#include <glog/logging.h>

#include "renderer/app.h"


DEFINE_bool(single_process, false, "Use a single process instead of the CEF original architecture");


int main(int argc, char* argv[]) {
  // Enable gflags
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Enable glog
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  CefMainArgs main_args(argc, argv);

  CefRefPtr<App> app(new App);

  // CEF shares the same executable for several process (render, plugins, GPU, etc.)
  // Run it now if we're one of them.
  int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
  if (exit_code >= 0) {
    return exit_code;
  }

  CefSettings settings;
  CefString(&settings.locales_dir_path) = "/renderer/cef/locales";
  CefString(&settings.user_agent) = "Renderer";
  CefString(&settings.browser_subprocess_path) = "bin/subprocess";
  settings.no_sandbox = true;
  settings.single_process = FLAGS_single_process;

  CefInitialize(main_args, settings, app.get(), NULL);

  CefRunMessageLoop();

  CefShutdown();

  return 0;
}
