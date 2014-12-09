// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/request.h"

#include "glog/logging.h"

#include "renderer/common/protobufs.h"


namespace seo {


Request::Request(uint64_t id, const std::string& url)
: id_(id), url_(url), output_stream_(STDOUT_FILENO) {
  // empty
}


void Request::EmitError(proto::seo::Response_Status status) {
  proto::seo::Response response;
  response.set_status(status);
  response.set_id(id_);

  Write_(response);
}


void Request::EmitSourceCode(const CefString& source_code) {
  proto::seo::Response response;
  response.set_status(proto::seo::Response_Status_OK);
  response.set_source_code(source_code.ToString());
  response.set_id(id_);

  Write_(response);
}


void Request::Write_(const proto::seo::Response& response) {
  if (!common::WriteDelimitedTo(response, &output_stream_)) {
    LOG(FATAL) << "cannot write response message to stdout";
  }

  // Flush stdout or otherwise some of the content may not appear in the output
  output_stream_.Flush();
}



}  // namespace seo
