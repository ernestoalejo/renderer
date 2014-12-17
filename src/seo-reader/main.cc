// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <iostream>

#include "glog/logging.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"

#include "base/protobufs.h"
#include "proto/seo/response.pb.h"


int main(int argc, char* argv[]) {
  // Check protobuf version
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Enable glog
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // Enable gflags
  google::ParseCommandLineFlags(&argc, &argv, true);

  // Read responses
  google::protobuf::io::FileInputStream input_stream(STDIN_FILENO);
  while (true) {
    proto::seo::Response response;
    if (!base::ReadDelimitedFrom(&input_stream, &response)) {
      // EOF is not a real error
      if (fgetc(stdin) == EOF) {
        break;
      }

      LOG(FATAL) << "cannot read delimited request";
    }

    // Log received data
    LOG(INFO) << "=================================================";
    LOG(INFO) << "response id: " << response.id();
    LOG(INFO) << "response read with status: " << response.status();
    LOG(INFO) << "url: " << response.url();
    LOG(INFO) << "source code length: " << response.source_code().size();
    LOG(INFO) << "source code start: " << response.source_code().substr(0, 80);
    LOG(INFO) << "=================================================";
  }

  // Shutdown protobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
