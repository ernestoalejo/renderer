// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <gflags/gflags.h>
#include <google/protobuf/stubs/common.h>

#include "renderer/common/initialize.h"
#include "renderer/seo/app.h"


int main(int argc, char* argv[]) {
  // Check protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  InitializeBeforeApp(argv);

  seo::App* app = new seo::App;

  int result = InitializeAfterApp(argc, argv, CefRefPtr<CefApp>(app));

  // Shutdown protobuf
  google::protobuf::ShutdownProtobufLibrary();

  return result;
}
