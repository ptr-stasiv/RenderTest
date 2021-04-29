#pragma once
#include <string_view>
#include <memory>

#include "math/math.h"
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

      virtual void Dispatch(uint32_t workGroupsX, uint32_t workGroupsY = 1, const uint32_t workGroupsZ = 1) = 0;

      virtual void AddInputBuffer(const std::shared_ptr<ShaderBuffer>& ssbo, const std::string_view& name) = 0;

      virtual void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Texture2D>& texture) const = 0;

      virtual void SetFloats(const std::string_view& name, const math::Matrix4& m) const = 0;

      virtual void SetFloats(const std::string_view& name, const mm::vec4& v) const = 0;

      virtual void SetFloats(const std::string_view& name, const mm::vec3& v) const = 0;

      virtual void SetFloat(const std::string_view& name, const float s) const = 0;

      virtual void SetInt(const std::string_view& name, const int32_t s) const = 0;   
   };
}
