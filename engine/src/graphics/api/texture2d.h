#pragma once
#include <cstdint>

#include "api.h"

namespace graphics
{
   enum class TextureWrap
   {
      ClampToEdge,
      ClampToBorder,
      Repeat
   };

   enum class TextureFilter
   {
      Nearest,
      Linear
   };

   struct TextureParams
   {
      TextureWrap WrapS;
      TextureWrap WrapT;
      TextureFilter MagFilter;
      TextureFilter MinFilter;
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
      
      //Through OpenGL api you should call this before init function
      virtual void SetBorderColor(const mm::vec4& color) const = 0;

      virtual uint16_t GetSizeX() const = 0;
      virtual uint16_t GetSizeY() const = 0;
   };
}
