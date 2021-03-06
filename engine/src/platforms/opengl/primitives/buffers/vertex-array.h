#pragma once
#include "buffer.h"

namespace gl
{
   struct VertexArray
   {
      GLuint BindId;
      GLuint BufferIndexCounter;
   };

   inline VertexArray CreateVertexArray()
   {
      GLuint bindId;
      glCreateVertexArrays(1, &bindId);

      return { bindId, 0 };
   }

   inline void DeleteVertexArray(const VertexArray& va)
   {
      glDeleteVertexArrays(1, &va.BindId);
   }

   inline GLuint AddBufferVertexArray(VertexArray& va, const VertexBuffer& vb, const GLsizei stride)
   {
      glVertexArrayVertexBuffer(va.BindId, va.BufferIndexCounter, vb.BindId, 0, stride);
      return va.BufferIndexCounter++;
   }

   inline void AddAttribFormatVertexArray(const VertexArray& va, const GLuint attrib, const GLuint buffer, const GLint elementsCount, GLenum type, bool normalized, GLuint offset)
   {
      glEnableVertexArrayAttrib(va.BindId, attrib);
      glVertexArrayAttribBinding(va.BindId, attrib, buffer);
      glVertexArrayAttribFormat(va.BindId, attrib, elementsCount, type, normalized, offset);
   }
}
