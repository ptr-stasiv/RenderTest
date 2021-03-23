#pragma once

namespace graphics
{
   class ShaderBuffer
   {
   public:
      ShaderBuffer() = default; 
      virtual ~ShaderBuffer() = default;

      virtual void InitData(const size_t size, const void* data) = 0;

      virtual void UpdateData(const size_t size, const void* data, const size_t offset = 0) = 0;
   };
}
