// Copyright (c) 2014 The renderer authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "renderer/common/visitors.h"


namespace common {


namespace {

class StringVisitor : public CefStringVisitor {
 public:
  explicit StringVisitor(
      const base::Callback<void(const CefString& source)>& callback)
  : callback_(callback) {
    // empty
  }

  virtual void Visit(const CefString& source) OVERRIDE {
    callback_.Run(source);
  }

 private:
  base::Callback<void(const CefString& source)> callback_;

  IMPLEMENT_REFCOUNTING(StringVisitor);
};

}  // namespace


CefRefPtr<CefStringVisitor> StringVisitorFromCallback(
    const base::Callback<void(const CefString& source)>& callback) {
  return new StringVisitor(callback);
}


}  // namespace common
