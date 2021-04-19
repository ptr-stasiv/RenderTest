#pragma once
#include "graphics/api/compute-shader.h"

#include "GL/glew.h"
#include "debug/globals.h"
#include "gl-texture2d.h"
#include "gl-shader-buffer.h"

namespace graphics
{
   namespace gl
   {
      class ComputeShaderGL : public ComputeShader
      {
      private:
         uint8_t SBufferCounter = 0;
      public:
         GLuint ProgramId;

         inline ComputeShaderGL() 
         {
            ProgramId = glCreateProgram();
         }

         inline ~ComputeShaderGL() override
         {
            glDeleteProgram(ProgramId);
         }

         inline void Attach(const std::string_view& shaderSrc)  override
         {
            GLuint shader = glCreateShader(GL_COMPUTE_SHADER);

            const char* src = shaderSrc.data();
            glShaderSource(shader, 1, &src, NULL);

            glAttachShader(ProgramId, shader);
            glDeleteShader(shader);

            glLinkProgram(ProgramId);
         }

         inline void Use() const override
         {
            glUseProgram(ProgramId);
         }

         inline void Dispatch(uint32_t workGroupsX, uint32_t workGroupsY = 0, const uint32_t workGroupsZ = 0) override
         {
            Use();
            glDispatchCompute(workGroupsX, workGroupsY, workGroupsZ);
         }
         
         inline void AddInputBuffer(const std::shared_ptr<ShaderBuffer>& ssbo, const std::string_view& name) override
         {
            auto& glSSBO = std::static_pointer_cast<ShaderBufferGL>(ssbo);

            GLuint blockId = glGetProgramResourceIndex(ProgramId, GL_SHADER_STORAGE_BLOCK, &name[0]);
            ASSERT(blockId >= 0, "Invalid SSBO id!");

            glShaderStorageBlockBinding(ProgramId, blockId, SBufferCounter);
            
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, glSSBO->BindId);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SBufferCounter, glSSBO->BindId);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

            ++SBufferCounter;
         }

         inline void SetTexture2D(const std::string_view& name, const std::shared_ptr<graphics::Texture2D>& texture) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            ASSERT(loc >= 0, "Invalid shader uniform!");

            auto& glTexture = std::static_pointer_cast<gl::Texture2dGL>(texture);

            glUniformHandleui64ARB(loc, glTexture->Handle);
         }

         inline void SetFloats(const std::string_view& name, const math::Matrix4& m) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            ASSERT(loc >= 0, "Invalid shader uniform!");

            glUniformMatrix4fv(loc, 1, GL_FALSE, m.Data);
         }

         inline void SetFloats(const std::string_view& name, const mm::vec4& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            ASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform4f(loc, v.x, v.y, v.z, v.w);
         }

         inline void SetFloats(const std::string_view& name, const mm::vec3& v) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            ASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform3f(loc, v.x, v.y, v.z);
         }

         inline void SetFloat(const std::string_view& name, const float s) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            ASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform1f(loc, s);
         }

         inline void SetInt(const std::string_view& name, const int32_t s) const override
         {
            int loc = glGetUniformLocation(ProgramId, name.data());
            ASSERT(loc >= 0, "Invalid shader uniform!");

            glUniform1i(loc, s);
         }
      };
   }
}
