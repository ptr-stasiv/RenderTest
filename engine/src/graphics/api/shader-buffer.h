#pragma once
#include <any>

namespace graphics
{
   class ShaderBuffer
   {
   public:
      ShaderBuffer() = default; 
      virtual ~ShaderBuffer() = default;

      virtual void InitData(const size_t size, const void* data) = 0;

      virtual void UpdateData(const size_t offset, const size_t size, void* data) = 0;
      
      virtual void GetData(const size_t offset, const size_t size, void* buffer) = 0;
   };
}
