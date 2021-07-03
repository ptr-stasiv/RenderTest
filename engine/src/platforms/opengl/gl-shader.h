#pragma once
#include <unordered_map>

#include "gl-api.h"
#include "graphics/api/shader-program.h"
#include "gl-texture2d.h"
#include "gl-cubemap.h"
#include "gl-vertex-buffer.h"
#include "gl-uniform-buffer.h"
#include "gl-shader-buffer.h"
#include "GL/glew.h"

#include "debug/globals.h"

//This can be useful to avoid crash because of uniform optimization
#if 0
   #define UNIFORM_ASSERT(condtion, ...) ASSERT(condtion, __VA_ARGS__)
#else
   #define UNIFORM_ASSERT(condition, ...)
#endif

namespace graphics
{
   inline std::unordered_map<ShaderType, GLenum> ShaderTypeLookupMap = 
   {
      { ShaderType::Vertex, GL_VERTEX_SHADER },
      { ShaderType::Fragment, GL_FRAGMENT_SHADER },
      { ShaderType::Geometry, GL_GEOMETRY_SHADER }
   };

#define OGL_SHADER_TYPE(t) ShaderTypeLookupMap.at(t)

   namespace gl
   {
      class ShaderProgramGL : public ShaderProgram
      {
      private:
         uint8_t BufferCounter = 0;
         uint8_t UBufferCounter = 0;
         uint8_t SBufferCounter = 0;
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
            ASSERT(static_cast<uint8_t>(type) < ShaderTypeLookupMap.size(), "Not implemented shader type is passed!");

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
            ASSERT(blockId >= 0, "Invalid UBO id!");

            glUniformBlockBinding(ProgramId, blockId, UBufferCounter);

            glBindBuffer(GL_UNIFORM_BUFFER, glUBO->BindId);
            glBindBufferRange(GL_UNIFORM_BUFFER, UBufferCounter, glUBO->BindId, static_cast<GLintptr>(dataOffset), dataSize);

            ++UBufferCounter;
         }

         inline void AddInputBuffer(const std::shared_ptr<ShaderBuffer>& ssbo, const std::string_view& name) override
         {
            auto& glSSBO = std::static_pointer_cast<ShaderBufferGL>(ssbo);

            GLuint blockId = glGetProgramResourceIndex(ProgramId, GL_SHADER_STORAGE_BLOCK, &name[0]);
            ASSERT(blockId != GL_INVALID_INDEX, "Invalid SSBO id!");

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, glSSBO->BindId);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SBufferCounter, glSSBO->BindId);
            glShaderStorageBlockBinding(ProgramId, blockId, SBufferCounter);

            ++SBufferCounter;
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
               glCompileShader(shader);

               GLint res;
               glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
               if (!res)
               {
                  char log[512];
                  glGetShaderInfoLog(shader, 512, NULL, log);
                  LOG_ERROR("Shader compilation error!\n\t%s", log)
               }

               glAttachShader(ProgramId, shader);
               glDeleteShader(shader);
            }

            glLinkProgram(ProgramId);

            GLint res;
            glGetProgramiv(ProgramId, GL_LINK_STATUS, &res);
            if (!res)
            {
               char log[512];
               glGetProgramInfoLog(ProgramId, 512, NULL, log);
               LOG_ERROR("Shader program link error!\n\t%s", log)
            }
         }

         inline void Use() const override
         {
            glUseProgram(ProgramId);
         }

         inline void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Texture2D>& texture) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            auto& glTexture = std::static_pointer_cast<gl::Texture2dGL>(texture);

            glUniformHandleui64ARB(loc, glTexture->Handle);
         }

         inline void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Cubemap>& cubemap) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            auto& glTexture = std::static_pointer_cast<gl::CubemapGL>(cubemap);

            glUniformHandleui64ARB(loc, glTexture->Handle);
         }

         inline void SetFloats(const std::string_view& name, const math::Matrix4& m) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data);
         }

         inline void SetFloats(const std::string_view& name, const mm::mat4& m) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniformMatrix4fv(loc, 1, GL_FALSE, mm::transpose(m).Data);
         }

         inline void SetFloats(const std::string_view& name, const mm::vec4& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniform4f(loc, v.x, v.y, v.z, v.w);
         }

         inline void SetFloats(const std::string_view& name, const mm::vec3& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniform3f(loc, v.x, v.y, v.z);
         }

         inline void SetFloats(const std::string_view& name, const mm::vec2& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniform2f(loc, v.x, v.y);
         }

         inline void SetFloat(const std::string_view& name, const float s) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniform1f(loc, s);
         }

         inline void SetInt(const std::string_view& name, const int32_t s) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniform1i(loc, s);
         }

         inline void SetInts(const std::string_view& name, const mm::ivec2& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            UNIFORM_ASSERT(loc >= 0, "Invalid shader uniform: %s", &name[0]);

            glUniform2i(loc, v.x, v.y);
         }
      };
   }
}
