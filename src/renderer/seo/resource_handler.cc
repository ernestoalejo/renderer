// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/seo/resource_handler.h"

#include "glog/logging.h"
#include "include/base/cef_bind.h"
#include "include/cef_urlrequest.h"

#include "renderer/common/tasks.h"

#include "base/util.h"

namespace seo {

ResourceHandler::ResourceHandler(Request* request)
: buffer_read_(0), complete_(false), request_(request) {
  // empty
}

void ResourceHandler::Cancel() {
  // empty
}

bool ResourceHandler::CanGetCookie(const CefCookie& cookie) {
  // Don't use any cookie in the request
  return false;
}

bool ResourceHandler::CanSetCookie(const CefCookie& cookie) {
  // Don't save any cookie
  return false;
}

void ResourceHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                         int64& response_length,
                                         CefString& redirect_url) {
  response_length = -1;

  // Use the |response| object to set the mime type, http status code and other optional header values.
}

bool ResourceHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                     CefRefPtr<CefCallback> callback) {
  // Run the request in the render thread to associate it with the current
  // browser instance
  auto task = base::Bind(&ResourceHandler::ProcessRequestUIThread_, this,
      request, callback);
  CefPostTask(TID_UI, common::TaskFromCallback(task));

  // We are process the request
  return true;
}

bool ResourceHandler::ReadResponse(void* data_out, int bytes_to_read,
                                   int& bytes_read,
                                   CefRefPtr<CefCallback> callback) {
  auto bytes_data_out = static_cast<unsigned char*>(data_out);
  
  if (complete_) {
    VLOG(3) << "read response; complete";
    bytes_read = 0;
    return false;
  }

  base::AutoLock lock_scope(lock_);

  int remaining = buffer_.size() - buffer_read_;
  bytes_read = std::min(bytes_to_read, remaining);

  if (bytes_read > 0) {
    VLOG(3) << "read response; bytes_to_read: " << bytes_to_read <<
        "; bytes_read: " << bytes_read;

    auto start = buffer_.begin() + buffer_read_;
    std::copy(start, start + bytes_read, bytes_data_out);
    buffer_read_ += bytes_read;

    return true;
  }

  VLOG(3) << "read response; waiting for data; bytes_read: " << bytes_read;

  read_response_callback_ = callback;

  return true;
}

bool ResourceHandler::GetAuthCredentials(bool is_proxy, const CefString& host,
                                         int port, const CefString& realm,
                                         const CefString& scheme,
                                         CefRefPtr<CefAuthCallback> callback) {
  // We're never authenticating ourselves
  return false;
}

void ResourceHandler::OnDownloadData(CefRefPtr<CefURLRequest> request,
                                     const void* data, size_t data_length) {
  base::AutoLock lock_scope(lock_);
  auto bytes_data = static_cast<const unsigned char*>(data);

  int size = buffer_.size() - buffer_read_;
  std::vector<unsigned char> new_buffer(size + data_length);

  auto old_end = std::copy(buffer_.begin() + buffer_read_,
      buffer_.end(), new_buffer.begin());
  std::copy(bytes_data, bytes_data + data_length, old_end);

  buffer_.swap(new_buffer);
  buffer_read_ = 0;

  CallReadResponseCallback_();

  VLOG(3) << "on download data; buffer length: " << buffer_.size() <<
      " bytes; data_length: " << data_length;
}

void ResourceHandler::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                         uint64 current, uint64 total) {
  // empty
}

void ResourceHandler::OnRequestComplete(CefRefPtr<CefURLRequest> request) {
  base::AutoLock lock_scope(lock_);

  VLOG(1) << "request complete: " << request->GetRequest()->GetURL().ToString();
  
  // Flag this resource request as complete
  complete_ = true;

  // Alert the request of one less resource loading on the page
  request_->DecrementPendingRequests();

  // Alert the read of our buffer to keep get the last chunk of buffered data
  CallReadResponseCallback_();
}

void ResourceHandler::OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                       uint64 current, uint64 total) {
  // empty
}

void ResourceHandler::ProcessRequestUIThread_(CefRefPtr<CefRequest> request,
                                              CefRefPtr<CefCallback> callback) {
  REQUIRE_UI_THREAD();

  base::AutoLock lock_scope(lock_);
  
  VLOG(3) << "sending request to: " << request->GetURL().ToString();

  CefURLRequest::Create(request, this);

  callback->Continue();
}

void ResourceHandler::CallReadResponseCallback_() {
  if (read_response_callback_ == nullptr) {
    return;
  }
  
  CefRefPtr<CefCallback> callback = read_response_callback_;
  read_response_callback_ = nullptr;
  callback->Continue();
}

}  // namespace seo
