#pragma once
#include <unordered_map>

#include "graphics/api/texture2d.h"
#include "gl-api.h"
#include "GL/glew.h"

namespace graphics
{
   namespace gl
   {
      inline std::unordered_map<TextureWrap, GLenum> TextureWrappLookupMap = 
      {
         { TextureWrap::ClampToEdge, GL_CLAMP_TO_EDGE },
         { TextureWrap::ClampToBorder, GL_CLAMP_TO_BORDER },
         { TextureWrap::Repeat, GL_REPEAT } 
      };

      inline std::unordered_map<TextureFilter, GLenum> TextureFilterLookupMap =
      {
         { TextureFilter::Nearest, GL_NEAREST },
         { TextureFilter::Linear, GL_LINEAR }
      };

#define OGL_TEXTURE_WRAP(w) TextureWrappLookupMap.at(w)
#define OGL_TEXTURE_FILTER(f) TextureFilterLookupMap.at(f)

      class Texture2dGL : public Texture2D
      {
      public:
         GLuint64 Handle;
         GLuint BindId;
         uint16_t SizeX;
         uint16_t SizeY;

         Format CurrentFormat;
         Type CurrentType;

         //This can't be done straight by passing texture params
         //Because I'm using here immutable textures
         mutable mm::vec4 BorderColor;

         Texture2dGL() = default;

         inline ~Texture2dGL() override
         {
            glDeleteTextures(1, &BindId);
         }

         inline void InitData(const uint16_t sizeX, const uint16_t sizeY,
                       const InternalFormat internalFormat, const Format format,
                       const Type type, const TextureParams& params) override
         {
            SizeX = sizeX;
            SizeY = sizeY;

            CurrentFormat = format;
            CurrentType = type;

            glCreateTextures(GL_TEXTURE_2D, 1, &BindId);
         
            glTextureParameteri(BindId, GL_TEXTURE_WRAP_S, OGL_TEXTURE_WRAP(params.WrapS));
            glTextureParameteri(BindId, GL_TEXTURE_WRAP_T, OGL_TEXTURE_WRAP(params.WrapT));
            glTextureParameteri(BindId, GL_TEXTURE_MIN_FILTER, OGL_TEXTURE_FILTER(params.MinFilter));
            glTextureParameteri(BindId, GL_TEXTURE_MAG_FILTER, OGL_TEXTURE_FILTER(params.MagFilter));

            glTextureParameterfv(BindId, GL_TEXTURE_BORDER_COLOR, BorderColor.Data);

            glTextureStorage2D(BindId, 1, OGL_IFORMAT(internalFormat), sizeX, sizeY);

            Handle = glGetTextureHandleARB(BindId);
            glMakeTextureHandleResidentARB(Handle);
         }
         
         inline void UpdateData(const size_t sizeX, const size_t sizeY, const void* data) override
         {
            glTextureSubImage2D(BindId, 0, 0, 0, sizeX, sizeY, OGL_FORMAT(CurrentFormat), OGL_TYPE(CurrentType), data);
         }

         inline void SetBorderColor(const mm::vec4& color) const override
         {
            BorderColor = color;
         }
         
         inline uint16_t GetSizeX() const override
         {
            return SizeX;
         }

         inline uint16_t GetSizeY() const override
         {
            return SizeY;
         }
      };
   }
}
