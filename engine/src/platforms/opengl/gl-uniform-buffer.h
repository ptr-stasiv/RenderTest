#pragma once
#include "graphics/api/uniform-buffer.h"
#include "GL/glew.h"

namespace graphics
{
   namespace gl
   {
      class UniformBufferGL : public UniformBuffer
      {
      public:
         GLuint BindId;

         UniformBufferGL()
         {
            glGenBuffers(1, &BindId);
         }

         ~UniformBufferGL() override
         {
            glDeleteBuffers(1, &BindId);
         }

         inline void InitData(const size_t size, const void* data) override
         {
            glBindBuffer(GL_UNIFORM_BUFFER, BindId);
            glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
         }

         inline void UpdateData(const size_t size, const void* data, const size_t offset = 0) override
         {
            glBindBuffer(GL_UNIFORM_BUFFER, BindId);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
         }
      };
   }
}
