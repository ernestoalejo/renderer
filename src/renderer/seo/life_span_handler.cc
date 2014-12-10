// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/life_span_handler.h"

#include "glog/logging.h"
#include "include/base/cef_lock.h"


namespace seo {


namespace {


base::Lock g_browsers_count_lock;
int g_browsers_count = 0;


}


LifeSpanHandler::LifeSpanHandler(Request* request)
: request_(request) {
  // empty
}


int LifeSpanHandler::CountOpenBrowsers() {
  base::AutoLock lock_scope(g_browsers_count_lock);

  return g_browsers_count;
}


void LifeSpanHandler::NewBrowserCreated() {
  base::AutoLock lock_scope(g_browsers_count_lock);
  g_browsers_count++;
  
  VLOG(2) << "new browser, " << g_browsers_count << " in total";
}


void LifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  request_->set_browser(browser);
}


bool LifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  base::AutoLock lock_scope(g_browsers_count_lock);
  g_browsers_count--;

  VLOG(2) << "removed browser, " << g_browsers_count << " in total";

  request_->set_browser(NULL);

  // Close the window inmediatly, we don't need to notify any parent window
  return false;
}


}  // namespace seo
