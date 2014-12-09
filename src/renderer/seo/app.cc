// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/app.h"

#include <iostream>
#include <thread>  // NOLINT

#include "glog/logging.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"

#include "base/util.h"
#include "proto/seo/request.pb.h"
#include "renderer/common/protobufs.h"
#include "renderer/seo/client.h"


namespace seo {


void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  std::thread thread(std::bind(&App::ReadRequests_, this));
  thread.detach();
}


void App::ReadRequests_() {
  LOG(INFO) << "waiting for requests...";

  google::protobuf::io::FileInputStream input_stream(STDIN_FILENO);
  while (true) {
    proto::seo::Request request;
    if (!common::ReadDelimitedFrom(&input_stream, &request)) {
      LOG(FATAL) << "cannot read delimited request";
    }

    VLOG(2) << "read command: " << request.command();

    // When we receive the exit command no more inputs are read from stdin
    // When the browsers finish they will close themselves and the app will
    // close too.
    if (request.command() == proto::seo::Request_Command_EXIT) {
      LOG(INFO) << "received exit";
      return;
    }

    Client* client = new Client(request.id(), request.url());
    client->Init();

    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;
    window_info.windowless_rendering_enabled = true;

    LOG(INFO) << "request url: " << request.url();
    CefBrowserHost::CreateBrowser(window_info, client, request.url(),
        browser_settings, NULL);
  }
}


}  // namespace seo
