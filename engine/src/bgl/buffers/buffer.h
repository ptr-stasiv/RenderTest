#pragma once
#include "GL/glew.h"

namespace bgl
{
   class Buffer
   {
   public:
      GLuint BindId;

      inline Buffer(const size_t size, const void* data)
      {
         glCreateBuffers(1, &BindId);
         glNamedBufferData(BindId, size, data, GL_STATIC_DRAW);
      }

      inline ~Buffer()
      {
         glDeleteBuffers(1, &BindId);
      }

      Buffer(const Buffer&) = delete;
      Buffer& operator = (const Buffer&) = delete;
   };
}