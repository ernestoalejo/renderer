// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.


int main(int argc, char* argv[]) {
  CefMainArgs main_args(argc, argv);

  // CEF shares the same executable for several process (render, plugins, GPU, etc.)
  // Run it now if we're one of them.
  // int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
  // if (exit_code >= 0) {
  //   return exit_code;
  // }

  CefShutdown();

  return 0;
}



// #include "cefsimple/simple_app.h"

// // Entry point function for all processes.
// int main(int argc, char* argv[]) {
//   // Provide CEF with command-line arguments.
//   CefMainArgs main_args(argc, argv);
  
//   // SimpleApp implements application-level callbacks. It will create the first
//   // browser instance in OnContextInitialized() after CEF has initialized.
//   CefRefPtr<SimpleApp> app(new SimpleApp);

//   // CEF applications have multiple sub-processes (render, plugin, GPU, etc)
//   // that share the same executable. This function checks the command-line and,
//   // if this is a sub-process, executes the appropriate logic.
//   int exit_code = CefExecuteProcess(main_args, app.get(), NULL);
//   if (exit_code >= 0) {
//     // The sub-process has completed so return here.
//     return exit_code;
//   }

//   // Initialize GTK.
//   gtk_init(&argc, &argv);

//   // Specify CEF global settings here.
//   CefSettings settings;

//   // Initialize CEF for the browser process.
//   CefInitialize(main_args, settings, app.get(), NULL);

//   // Run the CEF message loop. This will block until CefQuitMessageLoop() is
//   // called.
//   CefRunMessageLoop();

//   // Shut down CEF.
//   CefShutdown();

//   return 0;
// }
