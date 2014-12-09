// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_SEO_EMITTER_H_
#define RENDERER_SEO_EMITTER_H_

#include "google/protobuf/io/zero_copy_stream_impl.h"

#include "proto/seo/response.pb.h"
#include "include/internal/cef_string.h"


namespace seo {


class Emitter {
 public:
  Emitter(uint64_t id);

  void EmitError(Response_Status status);
  void EmitSourceCode(const CefString& source_code);

 private:
  uint64_t id_;

  google::protobuf::io::FileOutputStream output_stream_;

  void Write_(const Response& response);
};


}  // namespace seo


#endif  // RENDERER_SEO_EMITTER_H_
