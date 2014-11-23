// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <glog/logging.h>

#include "renderer/karma/app.h"


DEFINE_bool(karma, false, "activate karma app");
DEFINE_bool(seo, false, "activate seo app");


CefRefPtr<CefApp> GetCurrentApp() {
  if (FLAGS_karma) {
    CefRefPtr<CefApp> app(new karma::App);
    return app;
  }

  if (FLAGS_seo) {
    
  }

  LOG(FATAL) << "at least the karma or the seo should be active";
  return NULL;
}
