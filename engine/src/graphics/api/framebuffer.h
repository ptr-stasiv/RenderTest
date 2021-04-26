#pragma once
#include <memory>

#include "texture2d.h"
#include "api.h"

namespace graphics
{
   class Framebuffer
   {
      Framebuffer() = default;
      virtual ~Framebuffer() = default;

      virtual void AttachTexture2D(const Attachment attachment, const std::shared_ptr<Texture2D>& texture) = 0;
   };
}