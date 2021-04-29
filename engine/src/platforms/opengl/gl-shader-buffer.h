#pragma once
#include "graphics/api/shader-buffer.h"

#include "GL/glew.h"

namespace graphics
{
   namespace gl
   {
      class ShaderBufferGL : public ShaderBuffer
      {
      public:
         GLuint BindId;

         inline ShaderBufferGL()
         {
            glGenBuffers(1, &BindId);
         }

         inline ~ShaderBufferGL() override
         {
            glDeleteBuffers(1, &BindId);
         }

         inline void InitData(const size_t size, const void* data) override
         {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, BindId);
            glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
         }

         inline void UpdateData(const size_t size, const void* data, const size_t offset) override
         {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, BindId);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, static_cast<GLintptr>(offset), size, data);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
         }

         inline void GetData(const size_t offset, const size_t size, void** buffer) override
         {
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, BindId);
            glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, *buffer);
         }
      };
   }
}
