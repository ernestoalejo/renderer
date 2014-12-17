// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/resource_handler.h"

#include "glog/logging.h"
#include "include/base/cef_bind.h"
#include "include/cef_urlrequest.h"

#include "base/tasks.h"
#include "base/util.h"

namespace seo {

ResourceHandler::ResourceHandler(CefRefPtr<Request> request)
: request_(request) {
  // empty
}

void ResourceHandler::Cancel() {
  // empty
}

bool ResourceHandler::CanGetCookie(const CefCookie& cookie) {
  // Don't use any cookie in the request
  return false;
}

bool ResourceHandler::CanSetCookie(const CefCookie& cookie) {
  // Don't save any cookie
  return false;
}

void ResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                         int64& response_length,
                                         CefString& redirect_url) {
  response_length = -1;
}

bool ResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                     CefRefPtr<CefCallback> callback) {
  // We are process the request
  return true;
}

bool ResourceHandler::ReadResponse(void* data_out, int bytes_to_read,
                                   int& bytes_read,
                                   CefRefPtr<CefCallback> callback) {
  bytes_read = 0;
  return false;
}

}  // namespace seo
