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

  std::thread thread(std::bind(&App::ReadRequests, this));
  thread.detach();

}


void App::ReadRequests() {
  LOG(INFO) << "running requests reader";

  Request message;
  google::protobuf::io::IstreamInputStream inputStream(&std::cin);
  if (!common::ReadDelimitedFrom(&inputStream, &message)) {
    LOG(FATAL) << "cannot read delimited request";
  }

  CefBrowserSettings browser_settings;

  CefWindowInfo window_info;
  window_info.SetAsOffScreen(NULL);
  window_info.SetTransparentPainting(true);

  LOG(INFO) << "request url: " << message.url();
  CefBrowserHost::CreateBrowser(window_info, handler_.get(), message.url(),
      browser_settings, NULL);
}


}  // namespace seo
