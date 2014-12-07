// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <fstream>

#include <glog/logging.h>

#include "proto/seo/request.pb.h"


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

  std::fstream output("/tmp/request", std::ios::out | std::ios::trunc | std::ios::binary);
  if (!request.SerializeToOstream(&output)) {
    LOG(FATAL) << "failed to write request file";
  }

  LOG(INFO) << "request generated in /tmp/request";

  // Shutdown protobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
