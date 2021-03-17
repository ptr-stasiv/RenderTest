#pragma once
#include <cstdint>

#include "api.h"

namespace graphics
{
   struct TextureParams
   {
      uint16_t WrapS;
      uint16_t WrapT;
      uint16_t MagFilter;
      uint16_t MinFilter;
   };

   class Texture2D
   {
   public:
      Texture2D() = default;
      virtual ~Texture2D() = default;

      virtual void InitData(const uint16_t sizeX, const uint16_t sizeY,
                            const InternalFormat internalFormat, const Format format,
                            const Type type, const TextureParams& params) = 0;
      
      virtual void UpdateData(const size_t sizeX, const size_t sizeY, const void* data) = 0;
      
      virtual uint16_t GetSizeX() const = 0;
      virtual uint16_t GetSizeY() const = 0;
   };
}
