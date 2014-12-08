// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/common/tasks.h"


namespace common {


namespace {

class Task : public CefTask {
 public:
  explicit Task(const base::Callback<void()>& callback)
  : callback_(callback) {
    // empty
  }

  virtual void Execute() OVERRIDE {
    callback_.Run();
  }

 private:
  base::Callback<void()> callback_;

  IMPLEMENT_REFCOUNTING(Task);
};

}  // namespace


CefRefPtr<CefTask> TaskFromCallback(const base::Callback<void()>& callback) {
  return new Task(callback);
}


}  // namespace common
