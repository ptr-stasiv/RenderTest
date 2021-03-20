#pragma once
#include <string_view>
#include <memory>

#include "shader-program.h"

namespace graphics
{
   class UniformBuffer
   {
   public:
      UniformBuffer() = default;
      virtual ~UniformBuffer() = default;

      virtual void InitData(const size_t size, const void* data) = 0;

      virtual void UpdateData(const size_t size, const void* data, const size_t offset = 0) = 0;
   };
}
