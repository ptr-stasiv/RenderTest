#pragma once
#include <string_view>

#include "bgl/shader.h"

namespace graphics
{
   class ComputeShader
   {
   private:
      GLuint ProgramId;
   public:
      inline ComputeShader(const std::string_view& shaderSource)
      {
         GLuint shader = bgl::CreateShader(GL_COMPUTE_SHADER, shaderSource.data());
       
         ProgramId = glCreateProgram();

         glAttachShader(ProgramId, shader);

         glLinkProgram(ProgramId);

         glDeleteShader(shader);
      }

      inline ~ComputeShader()
      {
         glDeleteProgram(ProgramId);
      }

      inline void Use() const
      {
         glUseProgram(ProgramId);
      }

      inline void Dispatch(const GLuint numGroupsX = 1, GLuint numGroupsY = 1, const GLuint numGroupsZ = 1) const
      {
         Use();
         glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
      }

      inline void SetFloats(const std::string_view& name, const math::Matrix4& m) const
      {
         bgl::SetShaderMatrix4(ProgramId, &name[0], m);
      }

      inline void SetFloats(const std::string_view& name, const math::Vector4& v) const
      {
         bgl::SetShaderVector4(ProgramId, &name[0], v);
      }

      inline void SetFloats(const std::string_view& name, const math::Vector3& v) const
      {
         bgl::SetShaderVector3(ProgramId, &name[0], v);
      }

      inline void SetFloat(const std::string_view& name, const float s) const
      {
         bgl::SetShaderFloat(ProgramId, &name[0], s);
      }

      inline void SetInt(const std::string_view& name, const int32_t s) const
      {
         bgl::SetShaderInt(ProgramId, &name[0], s);
      }
   };
}