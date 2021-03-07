#pragma once
#include <string_view>
#include <memory>

#include "math/matrices/matrix4x4.h"
#include "math/vectors/vector4.h"
#include "math/vectors/vector3.h"

namespace graphics
{
   class ComputeShader
   {
   private:
      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;
   public:
      ComputeShader(const std::string_view& shaderSource);

      ~ComputeShader();

      void Use() const;

      void Dispatch(const uint32_t numGroupsX = 1, uint32_t numGroupsY = 1, const uint32_t numGroupsZ = 1) const;
      
      void SetFloats(const std::string_view& name, const math::Matrix4& m) const;
      
      void SetFloats(const std::string_view& name, const math::Vector4& v) const;

      void SetFloats(const std::string_view& name, const math::Vector3& v) const;

      void SetFloat(const std::string_view& name, const float s) const;

      void SetInt(const std::string_view& name, const int32_t s) const;
   };
}
