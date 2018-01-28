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

#ifndef _PASS_H_B3C6A58A_CD87_4D98_AB21_828ED88DBE28_
#define _PASS_H_B3C6A58A_CD87_4D98_AB21_828ED88DBE28_ 

#include <memory>
#include <bitset>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>
#include "shader.h"
#include "common/tags.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
struct BlendFactor {
  enum Value {
    kOne,
    kZero,
    kSrcColor,
    kSrcAlpha,
    kDstColor,
    kDstAlpha,
    kOneMinusSrcColor,
    kOneMinusSrcAlpha,
    kOneMinusDstColor,
    kOneMinusDstAlpha
  };

  static int ToOpenGL(Value v);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
struct BlendOp {
  enum Value {
    kAdd,
    kSub,
    kRevSub,
    kMin,
    kMax
  };

  static int ToOpenGL(Value v);
};

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
struct CullMode {
  enum Value {
    kCw,
    kCcw,
    kOff
  };
  
  static int ToOpenGL(Value v);
};


//////////////////////////////////////////////////////////////////////////////
// Render options such as ztest, cull and blend.
//////////////////////////////////////////////////////////////////////////////
class PassOptions {
 public:
  enum {
    kZwrite = 0,
    kZtest,
    kCull,
    kBlend,
    kClipping,

    kTotal
  };

  enum {
    kMaximumClippingPlanes = 6
  };

  PassOptions() : 
    zwrite_            (true),
    ztest_             (true),
    cull_              (CullMode::kCw),
    src_blend_factor_  (BlendFactor::kOne),
    dst_blend_factor_  (BlendFactor::kOne),
    blend_op_          (BlendOp::kAdd) {}

  void SetZwrite(bool on) {
    zwrite_ = on;
    mask_.set(kZwrite);
  }

  void SetZtest(bool on) {
    ztest_ = on;
    mask_.set(kZtest);
  }

  void SetCull(CullMode::Value cull) {
    cull_ = cull;
    mask_.set(kCull);
  }

  void SetSrcBlendFactor(BlendFactor::Value blend_factor) {
    src_blend_factor_ = blend_factor;
    mask_.set(kBlend);
  }
  
  void SetDstBlendFactor(BlendFactor::Value blend_factor) {
    dst_blend_factor_ = blend_factor;
    mask_.set(kBlend);
  }

  void SetBlendOp(BlendOp::Value blend_op) {
    blend_op_ = blend_op;
    mask_.set(kBlend);
  }

  void SetClippingPlanes(const std::vector<int>& clipping_planes) {
    clipping_planes_.clear();
    for (const auto& i: clipping_planes) {
      if (i > kMaximumClippingPlanes-1) {
        throw std::logic_error("PassOptions::SetClippingPlanes() - "
                               "maximum allowed clipping plane " +
                               std::to_string(kMaximumClippingPlanes-1) +
                               " while you're trying " + std::to_string(i));
      }
      clipping_planes_.insert(i);
    }
    mask_.set(kClipping);
  }

  void Bind();
  void Unbind();

 private:
  std::bitset<kTotal>   mask_;

  bool                  zwrite_;
  bool                  ztest_;
  CullMode::Value       cull_; 
  BlendFactor::Value    src_blend_factor_;
  BlendFactor::Value    dst_blend_factor_;
  BlendOp::Value        blend_op_;
  std::set<int>         clipping_planes_;
};


//////////////////////////////////////////////////////////////////////////////
// Single render pass 
//////////////////////////////////////////////////////////////////////////////
class Pass {
 public:
  PassOptions options;
  enum {kDefaultRenderQueue = 100};
  Pass() : name_("Unnamed"), queue_(kDefaultRenderQueue) {}

  void Bind();
  void Unbind();

  void SetName(const std::string& name) { name_ = name; }
  const std::string& GetName() { return name_; }

  void SetQueue(int queue) {queue_ = queue;}
  int  GetQueue() const {return queue_;}

  void SetShader(std::shared_ptr<Shader> shader) { shader_ = shader; }
  
  void SetTags(const std::vector<std::string>& tags) {
    tags_.Set(tags);
  }

  template <class T>
  bool CheckTags(T tag) const {
    return tags_.Check(tag);
  }

  // Bypass uniform to the shaders
  template <typename T>
  void SetUniform(const std::string& name, const T& value) {
    shader_->SetUniform(name, value);
  }

 public:
  std::string             name_;
  int                     queue_;
  std::shared_ptr<Shader> shader_;
  Tags                    tags_;
};


#endif // _PASS_H_B3C6A58A_CD87_4D98_AB21_828ED88DBE28_