#pragma once
#include <string_view>
#include <memory>

#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"
#include "math/matrices/matrix4x4.h"
#include "texture2d.h"
#include "shader-buffer.h"

namespace graphics
{
   class ComputeShader
   {
   public:
      ComputeShader() = default;
      virtual ~ComputeShader() = default;

      virtual void Attach(const std::string_view& shaderSrc)  = 0;

      virtual void Use() const = 0;

      virtual void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Texture2D>& texture) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Matrix4& m) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Vector4& v) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Vector3& v) const = 0;

      virtual void SetFloat(const std::string_view& name, const float s) const = 0;

      virtual void SetInt(const std::string_view& name, const int32_t s) const = 0;   
   };
}
