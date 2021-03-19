#pragma once
#include "graphics/api/vertex-buffer.h"
#include "GL/glew.h"

namespace graphics
{
   namespace gl
   {
      class VertexBufferGL : public VertexBuffer
      {
      public:
         GLuint BindId;

         inline VertexBufferGL()
         {
            glCreateBuffers(1, &BindId);
         }

         inline ~VertexBufferGL()
         {
            glDeleteBuffers(1, &BindId);
         }

         inline void InitData(const size_t size, const void* data) override
         {
            glNamedBufferData(BindId, size, data, GL_DYNAMIC_DRAW);
         }

         inline void UpdateData(const size_t size, const void* data) override
         {
            glNamedBufferSubData(BindId, 0, size, data);
         }
      };
   }
}
