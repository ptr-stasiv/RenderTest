#pragma once
#include <string_view>
#include <memory>

#include "math/matrices/matrix4x4.h"
#include "math/vectors/vector4.h"
#include "math/vectors/vector3.h"

namespace graphics
{   
   enum class ShaderType : uint8_t
   {
      Vertex,
      Fragment,

      LAST_ENUM_ELEMENT
   };

   struct ShaderProgram
   {
   private:
      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;
   public:
      ShaderProgram();

      ~ShaderProgram();

      void Add(const ShaderType shaderType, const std::string_view& shaderSrc); 

      void Compile();

      void Use() const;

      void SetFloats(const std::string_view& name, const math::Matrix4& m) const;

      void SetFloats(const std::string_view& name, const math::Vector4& v) const;

      void SetFloats(const std::string_view& name, const math::Vector3& v) const;

      void SetFloat(const std::string_view& name, const float s) const;

      void SetInt(const std::string_view& name, const int32_t s) const;
   };
}
