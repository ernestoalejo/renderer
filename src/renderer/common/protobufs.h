// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef RENDERER_COMMON_PROTOBUFS_H_
#define RENDERER_COMMON_PROTOBUFS_H_

#include <google/protobuf/message.h>


namespace common {


bool WriteDelimitedTo(const google::protobuf::MessageLite& message,
                      google::protobuf::io::ZeroCopyOutputStream* raw_output);

bool ReadDelimitedFrom(google::protobuf::io::ZeroCopyInputStream* raw_input,
                       google::protobuf::MessageLite* message);

}  // namespace common


#endif  // RENDERER_COMMON_PROTOBUFS_H_



