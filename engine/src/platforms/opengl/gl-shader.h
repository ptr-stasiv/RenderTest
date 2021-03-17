#pragma once
#include <unordered_map>

#include "debug/log/log.h"
#include "graphics/api/shader-program.h"
#include "GL/glew.h"

namespace graphics
{
   static std::unordered_map<ShaderType, GLenum> ShaderTypeLookupMap = 
   {
      { ShaderTypeVertex, GL_VERTEX_SHADER },
      { ShaderTypeFragment, GL_FRAGMENT_SHADER }
   };

#define OGL_SHADER_TYPE(t) ShaderTypeLookupMap.at(t)

   namespace gl
   {
      class ShaderProgramGL : public ShaderProgram
      {
      private:
         GLuint ProgramId;
         std::string_view ShaderArr[ShadersTypeCount];
      public:   
         inline ShaderProgramGL()
         {
            ProgramId = glCreateProgram();
         }

         inline ~ShaderProgramGL() override
         {
            glDeleteProgram(ProgramId);
         }

         inline void AddShader(const ShaderType& type, const std::string_view& src) override
         {
            if (static_cast<uint8_t>(type) >= ShadersTypeCount)
            {
               LOG_ERROR("Shader pipeline error!");
               return;
            }

            ShaderArr[static_cast<uint8_t>(type)] = src;
         }

         inline void AddInputBuffer(const VertexBuffer& vbo, const uint8_t attribIndex, const size_t offset, const size_t stride) override {}

         inline void Compile() override
         {

            for (uint8_t i = 0; i < ShadersTypeCount; ++i)
            {
               if (ShaderArr[i].empty())
                  continue;

               GLenum oglShaderType = OGL_SHADER_TYPE(ShaderType(i));

               GLuint shader = glCreateShader(oglShaderType);
               glShaderSource(shader, 1, (const char* const*)ShaderArr[i].data(), NULL);

               glAttachShader(ProgramId, shader);
               glDeleteShader(shader);
            }

            glLinkProgram(ProgramId);
         }

         inline void Use() override
         {
            glUseProgram(ProgramId);
         }

         inline void SetFloats(const std::string_view& name, const math::Matrix4& m) const override
         {
            glUniformMatrix4fv(glGetUniformLocation(ProgramId, name.data()), 1, GL_FALSE, m.Data);
         }

         inline void SetFloats(const std::string_view& name, const math::Vector4& v) const override
         {
            glUniform4f(glGetUniformLocation(ProgramId, name.data()), v.x, v.y, v.z, v.w);
         }

         inline void SetFloats(const std::string_view& name, const math::Vector3& v) const override
         {
            glUniform3f(glGetUniformLocation(ProgramId, name.data()), v.x, v.y, v.z);
         }

         inline void SetFloat(const std::string_view& name, const float s) const override
         {
            glUniform1f(glGetUniformLocation(ProgramId, name.data()), s);
         }

         inline void SetInt(const std::string_view& name, const int32_t s) const override
         {
            glUniform1i(glGetUniformLocation(ProgramId, name.data()), s);
         }
      };
   }
}
