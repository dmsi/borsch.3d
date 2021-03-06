//
// This source file is a part of borsch.3d
//
// Copyright (C) borsch.3d team 2017-2018
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef _ACTION_H_4D405188_755B_4E9D_A7B6_8A2D3596AD4E_
#define _ACTION_H_4D405188_755B_4E9D_A7B6_8A2D3596AD4E_ 

#include "transformation.h"
#include "appcontext.h"
#include <memory>

class Action {
 public:
  Action(std::shared_ptr<Transformation> t) : transform(t) {}
  virtual void Start() {}
  virtual void Update() {}
  virtual void PreDraw() {}
  virtual void PostDraw() {}

  std::shared_ptr<Transformation> GetTransform() {
    return transform;
  }

 protected:

  auto& GetActor() {
    return transform->GetActor();
  }

  //template <typename T, typename... TArgs>
  //auto GetComponent(TArgs&&... args) {
  //  return GetActor().GetComponent<T>(std::forward<TArgs>(args)...);
  //}

  const Timer& GetTimer() const {
    return AppContext::Instance().timer;
  }

  const Input& GetInput() const {
    return AppContext::Instance().input;
  }

  const Display& GetDisplay() const {
    return AppContext::Instance().display;
  }

  std::shared_ptr<Transformation> transform; 
};

#endif // _ACTION_H_4D405188_755B_4E9D_A7B6_8A2D3596AD4E_
