// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "base/protobufs.h"

#include "google/protobuf/io/coded_stream.h"

namespace base {

bool WriteDelimitedTo(const google::protobuf::MessageLite& message,
                      google::protobuf::io::ZeroCopyOutputStream* raw_output) {
  google::protobuf::io::CodedOutputStream output(raw_output);

  const int size = message.ByteSize();
  output.WriteVarint32(size);

  uint8_t* buffer = output.GetDirectBufferForNBytesAndAdvance(size);
  if (buffer != NULL) {
    // Optimization: The message fits in one buffer, so use the faster
    // direct-to-array serialization path
    message.SerializeWithCachedSizesToArray(buffer);
  } else {
    // Slightly-slower path when the message is multiple buffers.
    message.SerializeWithCachedSizes(&output);
    if (output.HadError()) {
      return false;
    }
  }

  return true;
}

bool ReadDelimitedFrom(google::protobuf::io::ZeroCopyInputStream* raw_input,
                       google::protobuf::MessageLite* message) {
  google::protobuf::io::CodedInputStream input(raw_input);

  // Limit the stream to the size prepended to the message
  uint32_t size;
  if (!input.ReadVarint32(&size)) {
    return false;
  }
  google::protobuf::io::CodedInputStream::Limit limit = input.PushLimit(size);

  if (!message->MergeFromCodedStream(&input)) {
    return false;
  }
  if (!input.ConsumedEntireMessage()) {
    return false;
  }

  input.PopLimit(limit);

  return true;
}

}  // namespace base
