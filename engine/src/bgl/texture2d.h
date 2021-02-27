#pragma once
#include "GL/glew.h"

namespace bgl
{
   struct Texture2D
   {
      GLuint BindId;
      GLuint SizeX;
      GLuint SizeY;

      GLenum Format;
      GLenum InternalFormat;
      GLenum Type;
   };

   struct TextureParams
   {
      GLenum WrapS;
      GLenum WrapT;
      GLenum MagFilter;
      GLenum MinFilter;
   };

   static Texture2D CreateTexture(const GLuint sizeX, const GLuint sizeY,
                           const GLenum internalFormat, const GLenum format, const GLenum type, 
                           const TextureParams& params)
   {
      GLuint bindId;
      glCreateTextures(GL_TEXTURE_2D, 1, &bindId);
      
      glTextureParameteri(bindId, GL_TEXTURE_WRAP_S, params.WrapS);
      glTextureParameteri(bindId, GL_TEXTURE_WRAP_T, params.WrapT);
      glTextureParameteri(bindId, GL_TEXTURE_MIN_FILTER, params.MinFilter);
      glTextureParameteri(bindId, GL_TEXTURE_MAG_FILTER, params.MagFilter);

      glTextureStorage2D(bindId, 1, internalFormat, sizeX, sizeY);

      Texture2D texture;
      texture.BindId = bindId;
      texture.SizeX = sizeX;
      texture.SizeY = sizeY;
      texture.Format = format;
      texture.InternalFormat = internalFormat;
      texture.Type = type;

      return texture;
   }

   static void UpdateTexture2D(const Texture2D& texture, 
                        const GLuint sizeX, const GLuint sizeY, const void* data)
   {
      if (sizeX <= texture.SizeX
          || sizeY <= texture.SizeY)
      {
         glTextureSubImage2D(texture.BindId, 0, 0, 0, sizeX, sizeY, texture.Format, texture.Type, data);
      }
      else
      {
         glTextureStorage2D(texture.BindId, 1, texture.InternalFormat, sizeX, sizeY);
         glTextureSubImage2D(texture.BindId, 0, 0, 0, sizeX, sizeY, texture.Format, texture.Type, data);
      }
   }

   inline void DeleteTexture2D(const Texture2D& texture)
   {
      glDeleteTextures(1, &texture.BindId);
   }

   inline void BindTexture2D(const Texture2D& texture, const GLuint unit)
   {
      glBindTextureUnit(unit, texture.BindId);
   }
}