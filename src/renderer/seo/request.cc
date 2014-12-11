// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/request.h"

#include "glog/logging.h"
#include "include/base/cef_bind.h"

#include "base/util.h"
#include "renderer/common/protobufs.h"
#include "renderer/common/tasks.h"


namespace seo {


Request::Request(uint64_t id, const std::string& url)
: id_(id), url_(url), output_stream_(STDOUT_FILENO), failed_(false),
  closing_(false), pending_requests_(0) {
  // empty
}


void Request::EmitError(proto::seo::Response_Status status) {
  failed_ = true;

  proto::seo::Response response;
  response.set_id(id_);
  response.set_status(status);

  Write_(response);
  CloseBrowser_();
}


void Request::EmitSourceCode(const CefString& source_code) {
  proto::seo::Response response;
  response.set_id(id_);
  response.set_status(proto::seo::Response_Status_OK);
  response.set_source_code(source_code.ToString());

  Write_(response);
  CloseBrowser_();
}


void Request::EmitRedirection(const std::string& url) {
  proto::seo::Response response;
  response.set_id(id_);
  response.set_status(proto::seo::Response_Status_REDIRECT);
  response.set_url(url);

  Write_(response);
  CloseBrowser_();
}


void Request::CloseBrowser_() {
  closing_ = true;

  // Send the close order to the browser window in the correct thread
  auto callback = base::Bind(&Request::CloseBrowserUIThread_, this);
  CefPostTask(TID_UI, common::TaskFromCallback(callback));
}


void Request::CloseBrowserUIThread_() {
  REQUIRE_UI_THREAD();

  VLOG(2) << "close browser from the UI thread";
  browser_->GetHost()->CloseBrowser(true);
}


void Request::Write_(const proto::seo::Response& response) {
  if (!common::WriteDelimitedTo(response, &output_stream_)) {
    LOG(FATAL) << "cannot write response message to stdout";
  }

  // Flush stdout or otherwise some of the content may not appear in the output
  output_stream_.Flush();
}



}  // namespace seo
