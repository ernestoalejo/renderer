// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <iostream>

#include <glog/logging.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "proto/seo/response.pb.h"
#include "renderer/common/protobufs.h"


int main(int argc, char* argv[]) {
  // Check protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Enable glog
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // Enable gflags  
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Read responses
  seo::Response response;
  google::protobuf::io::IstreamInputStream inputStream(&std::cin);
  if (!common::ReadDelimitedFrom(&inputStream, &response)) {
    LOG(FATAL) << "cannot read delimited request";
  }

  // Log received data
  LOG(INFO) << "response read with status: " << response.status();
  LOG(INFO) << "source code length: " << response.source_code();

  // Shutdown protobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
