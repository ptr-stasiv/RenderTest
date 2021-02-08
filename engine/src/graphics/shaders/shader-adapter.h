#pragma once
#include <string_view>

#include "bgl/shader.h"
#include "math/matrices/matrix4x4.h"
#include "math/vectors/vector4.h"
#include "math/vectors/vector3.h"

namespace graphics
{
   struct Shader
   {
   private:
      GLuint ShaderId;
   public:
      Shader() = default;

      inline ~Shader()
      {
         glDeleteProgram(ShaderId);
      }

      inline void Use()
      {
         glUseProgram(ShaderId);
      }

      inline void SetFloats(const std::string_view& name, const math::Matrix4& m)
      {
         SetShaderMatrix4(ShaderId, &name[0], m);
      }

      inline void SetFloats(const std::string_view& name, const math::Vector4& v)
      {
         SetShaderVector4(ShaderId, &name[0], v);
      }

      inline void SetFloats(const std::string_view& name, const math::Vector3& v)
      {
         SetShaderVector3(ShaderId, &name[0], v);
      }

      inline void SetFloat(const std::string_view& name, const float s)
      {
         SetShaderFloat(ShaderId, &name[0], s);
      }

      inline void SetInt(const std::string_view& name, const int32_t s)
      {
         SetShaderInt(ShaderId, &name[0], s);
      }

      Shader(const Shader&) = default;
      Shader(Shader&&)      = default;
      Shader& operator = (const Shader&) = default;
      Shader& operator = (Shader&&)      = default;
   };
}