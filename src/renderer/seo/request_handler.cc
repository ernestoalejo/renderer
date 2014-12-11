// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/request_handler.h"

#include <string>
#include <fstream>

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "include/cef_url.h"

DEFINE_string(blacklisted_domains, "",
    "file with a list (one per line) of blacklisted domains");

namespace seo {

RequestHandler::RequestHandler(Request* request)
: request_(request) {
  // empty
}

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
  if (request_->failed() || request_->closing()) {
    VLOG(1) << "blocked resource because request failed or we are closing: " <<
        request->GetURL().ToString();
    return true;
  }

  auto rt = request->GetResourceType();
  if (rt == RT_STYLESHEET || rt == RT_IMAGE || rt == RT_FONT_RESOURCE ||
      rt == RT_FAVICON) {
    VLOG(1) << "blocked resource by type (" << rt << "): " <<
        request->GetURL().ToString();
    return true;
  }

  CefURLParts parts;
  bool valid = CefParseURL(request->GetURL(), parts);
  DCHECK(valid);

  CefString host(&parts.host);
  for (unsigned i = 0; i < blacklisted_domains_.size(); ++i) {
    if (blacklisted_domains_[i] == host.ToString()) {
      VLOG(1) << "blocked resource by blacklist: " <<
          request->GetURL().ToString();
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
  request_->IncrementPendingRequests();
  return new ResourceHandler(request_);
}

void RequestHandler::Init() {
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

      VLOG(2) << "blacklisted domain: " << line;
      blacklisted_domains_.push_back(line);
    }
  }
}

}  // namespace seo
