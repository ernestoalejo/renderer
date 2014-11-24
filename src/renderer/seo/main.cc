// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <gflags/gflags.h>

#include "renderer/seo/app.h"


int main(int argc, char* argv[]) {
  InitializeBeforeApp();
  InitializeAfterApp(new seo::App);

  return 0;
}
