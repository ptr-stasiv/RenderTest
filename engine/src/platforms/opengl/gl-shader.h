#pragma once
#include <unordered_map>

#include "debug/log/log.h"
#include "gl-api.h"
#include "graphics/api/shader-program.h"
#include "gl-texture2d.h"
#include "gl-vertex-buffer.h"
#include "gl-uniform-buffer.h"
#include "GL/glew.h"

namespace graphics
{
   inline std::unordered_map<ShaderType, GLenum> ShaderTypeLookupMap = 
   {
      { ShaderType::Vertex, GL_VERTEX_SHADER },
      { ShaderType::Fragment, GL_FRAGMENT_SHADER }
   };

#define OGL_SHADER_TYPE(t) ShaderTypeLookupMap.at(t)

   namespace gl
   {
      class ShaderProgramGL : public ShaderProgram
      {
      private:
         uint8_t BufferCounter = 0;
         uint8_t UBufferCounter = 0;
      public:   
         GLuint ProgramId;
         GLuint Vao;

         std::string_view ShaderArr[ShadersTypeCount];

         inline ShaderProgramGL()
         {
            ProgramId = glCreateProgram();

            glCreateVertexArrays(1, &Vao);
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

         inline void AddInputBuffer(const std::shared_ptr<VertexBuffer>& vbo, const uint8_t elements, const uint8_t attribIndex,
                                    const size_t stride, const Type type, const size_t offset = 0, const size_t elementsOffset = 0) override
         {
            auto& glVbo = std::static_pointer_cast<VertexBufferGL>(vbo);

            glVertexArrayVertexBuffer(Vao, BufferCounter, glVbo->BindId, offset, stride);

            glEnableVertexArrayAttrib(Vao, attribIndex);
            glVertexArrayAttribBinding(Vao, attribIndex, BufferCounter);
            glVertexArrayAttribFormat(Vao, attribIndex, elements, OGL_TYPE(type), GL_FALSE, elementsOffset);

            ++BufferCounter;
         }

         inline void AddInputBuffer(const std::shared_ptr<UniformBuffer>& ubo, const std::string_view& name, const size_t dataSize, const size_t dataOffset = 0) override
         {
            auto& glUBO = std::static_pointer_cast<UniformBufferGL>(ubo);

            GLuint blockId = glGetUniformBlockIndex(ProgramId, &name[0]);

            glUniformBlockBinding(ProgramId, blockId, UBufferCounter);

            glBindBuffer(GL_UNIFORM_BUFFER, glUBO->BindId);
            glBindBufferRange(GL_UNIFORM_BUFFER, UBufferCounter, glUBO->BindId, static_cast<GLintptr>(dataOffset), dataSize);

            ++UBufferCounter;
         }

         inline void Compile() override
         {
            for (uint8_t i = 0; i < ShadersTypeCount; ++i)
            {
               if (ShaderArr[i].empty())
                  continue;

               GLenum oglShaderType = OGL_SHADER_TYPE(ShaderType(i));

               GLuint shader = glCreateShader(oglShaderType);

               const char* src = ShaderArr[i].data();
               glShaderSource(shader, 1, &src, NULL);

               glAttachShader(ProgramId, shader);
               glDeleteShader(shader);
            }

            glLinkProgram(ProgramId);
         }

         inline void Use() const override
         {
            glUseProgram(ProgramId);
         }

         inline void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Texture2D>& texture) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            GASSERT(loc >= 0, "Invalid shader uniform!");

            auto& glTexture = std::static_pointer_cast<gl::Texture2dGL>(texture);

            glUniformHandleui64ARB(loc, glTexture->Handle);
         }

         inline void SetFloats(const std::string_view& name, const math::Matrix4& m) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            GASSERT(loc >= 0, "Invalid shader uniform!");

            glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data);
         }

         inline void SetFloats(const std::string_view& name, const math::Vector4& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            GASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform4f(loc, v.x, v.y, v.z, v.w);
         }

         inline void SetFloats(const std::string_view& name, const math::Vector3& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            GASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform3f(loc, v.x, v.y, v.z);
         }

         inline void SetFloat(const std::string_view& name, const float s) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            GASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform1f(loc, s);
         }

         inline void SetInt(const std::string_view& name, const int32_t s) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            GASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform1i(loc, s);
         }
      };
   }
}
