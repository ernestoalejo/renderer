// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/emitter.h"

#include "glog/logging.h"

#include "renderer/common/protobufs.h"


namespace seo {


Emitter::Emitter(uint64_t id)
: id_(id), output_stream_(STDOUT_FILENO) {
  // empty
}


void Emitter::EmitError(Response_Status status) {
  Response response;
  response.set_status(status);
  response.set_id(id_);

  Write_(response);
}


void Emitter::EmitSourceCode(const CefString& source_code) {
  Response response;
  response.set_status(Response_Status_OK);
  response.set_source_code(source_code.ToString());
  response.set_id(id_);

  Write_(response);
}


void Emitter::Write_(const Response& response) {
  if (!common::WriteDelimitedTo(response, &output_stream_)) {
    LOG(FATAL) << "cannot write response message to stdout";
  }

  // Flush stdout or otherwise some of the content may not appear in the output
  output_stream_.Flush();
}


}  // namespace seo
