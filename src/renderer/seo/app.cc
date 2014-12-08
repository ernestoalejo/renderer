// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/app.h"

#include <iostream>
#include <thread>

#include <glog/logging.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include "base/util.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "proto/seo/request.pb.h"
#include "renderer/common/protobufs.h"


namespace seo {


void App::OnContextInitialized() {
  REQUIRE_UI_THREAD();

  render_handler_ = new common::RenderHandler(1900, 800);
  request_handler_ = new RequestHandler;
  request_handler_->Initialize();

  std::thread thread(std::bind(&App::ReadRequests_, this));
  thread.detach();
}


void App::ReadRequests_() {
  LOG(INFO) << "waiting for requests...";

  google::protobuf::io::FileInputStream input_stream(STDIN_FILENO);
  while (true) {
    Request request;
    if (!common::ReadDelimitedFrom(&input_stream, &request)) {
      LOG(FATAL) << "cannot read delimited request";
    }

    VLOG(2) << "read command: " << request.command();

    if (request.command() == Request_Command_EXIT) {
      LOG(INFO) << "received exit";
      ExitAllHandlers();
      return;
    }

    Handler* handler = new Handler(render_handler_, request_handler_,
        request.id());
    CountNewHandler();

    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;

    LOG(INFO) << "request url: " << request.url();
    CefBrowserHost::CreateBrowser(window_info, handler, request.url(),
        browser_settings, NULL);
  }
}


}  // namespace seo
