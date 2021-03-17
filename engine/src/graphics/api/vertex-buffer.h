#pragma once
#include <any>

namespace graphics
{
   class VertexBuffer
   {
   public:
      VertexBuffer() = default;
      virtual ~VertexBuffer() = default;

      virtual void InitData(const size_t size, const void* data) = 0;

      virtual void UpdateData(const size_t size, const void* data) = 0;
   };
}
