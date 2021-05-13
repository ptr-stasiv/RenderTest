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
         size_t Size;

         inline ShaderBufferGL()
         {
            glCreateBuffers(1, &BindId);
         }

         inline ~ShaderBufferGL() override
         {
            glDeleteBuffers(1, &BindId);
         }

         inline void InitData(const size_t size, const void* data) override
         {
            glNamedBufferData(BindId, size, data, GL_DYNAMIC_DRAW);
            Size = size;
         }

         inline void UpdateData(const size_t offset, const size_t size, void* data) override
         {
            ASSERT(offset + size <= Size, "Update data is too big!");

            glNamedBufferSubData(BindId, static_cast<GLintptr>(offset), size, data);
         }

         inline void GetData(const size_t offset, const size_t size, void* buffer) override
         {
            ASSERT(offset + size <= Size, "Offset + size is out of buffer range!");

            glGetNamedBufferSubData(BindId, 0, size, buffer);
         }
      };
   }
}
