// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef BASE_PROTOBUFS_H_
#define BASE_PROTOBUFS_H_

#include <google/protobuf/message.h>

namespace base {

bool WriteDelimitedTo(const google::protobuf::MessageLite& message,
                      google::protobuf::io::ZeroCopyOutputStream* raw_output);

bool ReadDelimitedFrom(google::protobuf::io::ZeroCopyInputStream* raw_input,
                       google::protobuf::MessageLite* message);

}  // namespace base

#endif  // BASE_PROTOBUFS_H_



