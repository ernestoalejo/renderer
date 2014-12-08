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

  CefRefPtr<common::RenderHandler> render_handler = new common::RenderHandler(1900, 800);
  CefRefPtr<RequestHandler> request_handler = new RequestHandler;
  request_handler->Initialize();

  handler_ = new Handler(render_handler, request_handler);

  std::thread thread(std::bind(&App::ReadRequests_, this));
  thread.detach();

}


void App::ReadRequests_() {
  LOG(INFO) << "running requests reader";

  google::protobuf::io::IstreamInputStream inputStream(&std::cin);
  while (true) {
    Request request;
    if (!common::ReadDelimitedFrom(&inputStream, &request)) {
      LOG(FATAL) << "cannot read delimited request";
    }

    VLOG(2) << "read command: " << request.command();

    if (request.command() == Request_Command_EXIT) {
      handler_->Exit();
      return;
    }

    handler_->CountPendingRequest();

    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;

    LOG(INFO) << "request url: " << request.url();
    CefBrowserHost::CreateBrowser(window_info, handler_.get(), request.url(),
        browser_settings, NULL);
  }
}


}  // namespace seo
