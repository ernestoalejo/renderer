// Copyright (c) 2014 The renderer Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/request_handler.h"

#include <glog/logging.h>

#include "include/cef_url.h"


bool RequestHandler::OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info) {
  LOG(WARNING) << "plugin blocked: " << info->GetName().ToString();
  return true;
}

bool RequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request) {
  CefURLParts parts;
  bool valid = CefParseURL(request->GetURL(), parts);
  DCHECK(valid);

  CefString host(&parts.host);

  VLOG(1) << "resource request: " << request->GetURL().ToString();

  return false;
}
