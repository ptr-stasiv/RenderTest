#pragma once
#include "buffer.h"

namespace bgl
{
   class VertexArray
   {
   private:
      GLuint BufferIndexCounter = 0;
   public:
      GLuint BindId;

      inline VertexArray()
      {
         glCreateVertexArrays(1, &BindId);
      }

      inline ~VertexArray()
      {
         glDeleteVertexArrays(1, &BindId);
      }

      GLuint LinkBuffer(const Buffer& buffer, const GLsizei stride)
      {
         glVertexArrayVertexBuffer(BindId, BufferIndexCounter, buffer.BindId, 0, stride);
         return BufferIndexCounter++;
      }

      void AddAttribFormat(const GLuint attrib, const GLuint buffer, const GLint elementsCount, GLenum type, bool normalized, GLuint offset)
      {
         glEnableVertexArrayAttrib(BindId, attrib);
         glVertexArrayAttribBinding(BindId, attrib, buffer);
         glVertexArrayAttribFormat(BindId, attrib, elementsCount, type, normalized, offset);
      }
   };
}