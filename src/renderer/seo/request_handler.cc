// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/request_handler.h"

#include <string>
#include <fstream>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "include/cef_url.h"

#include "base/util.h"

DEFINE_string(blacklisted_domains, "",
    "file with a list (one per line) of blacklisted domains");

namespace seo {

RequestHandler::RequestHandler(CefRefPtr<Request> request)
: request_(request) {
  // empty
}

bool RequestHandler::OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info) {
  REQUIRE_IO_THREAD();

  LOG(WARNING) << "plugin blocked: " << info->GetName().ToString();
  return true;
}

bool RequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request) {
  REQUIRE_IO_THREAD();
  
  if (request_->failed() || request_->closing()) {
    VLOG(1) << "blocked resource because request failed or we are closing: " <<
        request->GetURL().ToString();
    return true;
  }

  std::string requestURL(request->GetURL().ToString());
  auto rt = request->GetResourceType();
  switch (rt) {
  case RT_STYLESHEET:
    VLOG(1) << "blocked stylesheet: " << requestURL;
    return true;

  case RT_IMAGE:
    VLOG(1) << "blocked image: " << requestURL;
    return true;

  case RT_FONT_RESOURCE:
    VLOG(1) << "blocked font: " << requestURL;
    return true;

  case RT_FAVICON:
    VLOG(1) << "blocked favicon: " << requestURL;
    return true;

  case RT_OBJECT:
    VLOG(1) << "block object: " << requestURL;
    return true;

  case RT_MEDIA:
    VLOG(1) << "block object: " << requestURL;
    return true;

  default:;
    // Keep the processing
  }

  CefURLParts parts;
  bool valid = CefParseURL(request->GetURL(), parts);
  DCHECK(valid);

  CefString host(&parts.host);
  for (unsigned i = 0; i < blacklisted_domains_.size(); ++i) {
    if (blacklisted_domains_[i] == host.ToString()) {
      VLOG(1) << "blacklisted resource: " << requestURL;
      return true;
    }
  }

  VLOG(1) << "resource request: " << request->GetURL().ToString();

  return false;
}

void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) {
  REQUIRE_IO_THREAD();
  
  if (request_->failed() || request_->closing() || !frame->IsMain()) {
    return;
  }

  LOG(INFO) << "redirect detected: " << old_url.ToString() << " -> " <<
      new_url.ToString();
  request_->EmitRedirection(new_url.ToString());
}

CefRefPtr<CefResourceHandler> RequestHandler::GetResourceHandler(
      CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request) {
  REQUIRE_IO_THREAD();
  
  request_->IncrementPendingRequests();
  return new ResourceHandler(request_);
}

void RequestHandler::Init() {
  REQUIRE_FILE_THREAD();

  std::string blacklisted_domains(FLAGS_blacklisted_domains);
  VLOG(2) << "blacklisted domains file: " << blacklisted_domains;

  if (!blacklisted_domains.empty()) {
    std::ifstream file(blacklisted_domains.c_str());
    if (!file) {
      LOG(FATAL) << "cannot find the blacklisted domains file in: "
          << blacklisted_domains;
    }

    std::string line;
    while (std::getline(file, line)) {
      // Ignore empty lines
      if (line.empty()) {
        continue;
      }

      // Ignore comments
      if (line[0] == '#') {
        continue;
      }

      VLOG(2) << "blacklisted domain: " << line;
      blacklisted_domains_.push_back(line);
    }
  }
}

}  // namespace seo
