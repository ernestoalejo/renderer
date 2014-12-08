// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include <iostream>

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

  google::protobuf::io::FileOutputStream output_stream(STDOUT_FILENO);

  {
    // Write example request
    seo::Request request;
    request.set_id(0);
    request.set_command(seo::Request_Command_GET_SOURCE_CODE);
    request.set_url("http://magpcss.org/ceforum/apidocs3/projects/(default)/CefBrowserHost.html");
    if (!common::WriteDelimitedTo(request, &output_stream)) {
      LOG(FATAL) << "cannot write message to the output file";
    }
  }

  {
    // Write example request
    seo::Request request;
    request.set_id(1);
    request.set_command(seo::Request_Command_GET_SOURCE_CODE);
    request.set_url("http://www.google.com/");
    if (!common::WriteDelimitedTo(request, &output_stream)) {
      LOG(FATAL) << "cannot write message to the output file";
    }
  }

  {
    // Write exit request
    seo::Request request;
    request.set_command(seo::Request_Command_EXIT);
    if (!common::WriteDelimitedTo(request, &output_stream)) {
      LOG(FATAL) << "cannot write message to the output file";
    }
  }

  LOG(INFO) << "requests generated";

  // Shutdown protobuf
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
