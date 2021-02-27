#pragma once
#include "GL/glew.h"

namespace bgl
{
   struct VertexBuffer
   {
      GLuint BindId;
      size_t Size;
   };

   inline VertexBuffer CreateVertexBuffer(const size_t size, const void* data)
   {
      GLuint bindId;
      glCreateBuffers(1, &bindId);
      glNamedBufferData(bindId, size, data, GL_STATIC_DRAW);

      return { bindId, size };
   }

   inline void DeleteVertexBuffer(const VertexBuffer& buffer)
   {
      glDeleteBuffers(1, &buffer.BindId);
   }
}