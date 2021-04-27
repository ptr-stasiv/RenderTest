#pragma once
#include "GL/glew.h"
#include "gl-api.h"
#include "gl-texture2d.h"
#include "graphics/api/framebuffer.h"

namespace graphics
{
   namespace gl
   {
      class FramebufferGL : public Framebuffer
      {
      public:
         GLuint BindId;

         inline FramebufferGL()
         {
            glCreateFramebuffers(1, &BindId);
         }

         inline ~FramebufferGL() override
         {
            glDeleteFramebuffers(1, &BindId);
         }

         inline void Bind() const override
         {
            glBindFramebuffer(GL_FRAMEBUFFER, BindId);
         }

         inline void Unbind() const override
         {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
         }

         inline void AttachTexture2D(const Attachment attachment, const std::shared_ptr<Texture2D>& texture) override
         {
            auto glTex = std::static_pointer_cast<Texture2dGL>(texture);
            glNamedFramebufferTexture2DEXT(BindId, OGL_ATTACHMENT(attachment), GL_TEXTURE_2D, glTex->BindId, 0);
         }
      };
   }
}
