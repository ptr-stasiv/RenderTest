#pragma once
#include <memory>

#include "texture2d.h"
#include "cubemap.h"
#include "api.h"

namespace graphics
{
   class Framebuffer
   {
   public:
      Framebuffer() = default;
      virtual ~Framebuffer() = default;

      virtual void Bind() const = 0;
      virtual void Unbind() const = 0;

      virtual void AttachTexture2D(const Attachment attachment, const std::shared_ptr<Texture2D>& texture) = 0;
      virtual void AttachTexture2D(const Attachment attachment, const CubeFace face, const std::shared_ptr<Cubemap>& cubemap) = 0;
   };
}
