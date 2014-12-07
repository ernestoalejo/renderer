// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <fstream>

#include <glog/logging.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "proto/seo/request.pb.h"
#include "renderer/common/protobufs.h"


int main(int argc, char* argv[]) {
  // Check protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Enable glog
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // Enable gflags  
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Write example request
  seo::Request request;
  request.set_url("http://www.google.com/");

  std::ofstream output("/tmp/request", std::ios::trunc | std::ios::binary);
  google::protobuf::io::OstreamOutputStream outputStream(&output);
  if (!common::WriteDelimitedTo(request, &outputStream)) {
    LOG(FATAL) << "cannot write message to the output file";
  }

  LOG(INFO) << "request generated in /tmp/request";

  // Shutdown protobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
