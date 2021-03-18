#pragma once
#include <unordered_map>

#include "GL/glew.h"
#include "graphics/api/api.h"

namespace graphics
{
   namespace gl
   {
      inline std::unordered_map<InternalFormat, GLenum> InternalFormatLookupMap = 
      {
         { InternalFormat::RGB8, GL_RGB8 },
         { InternalFormat::RGBA8, GL_RGBA8 },
         { InternalFormat::RGB16, GL_RGB16 },
         { InternalFormat::RGBA16, GL_RGBA16 }
      };

      inline std::unordered_map<Format, GLenum> FormatLookupMap =
      {
         { Format::RGB, GL_RGB },
         { Format::RGBA, GL_RGBA },
         { Format::BGR, GL_BGR },
         { Format::BGRA, GL_BGRA }
      };

      inline std::unordered_map<Type, GLenum> TypeLookupMap =
      {
         { Type::Ubyte, GL_UNSIGNED_BYTE },
         { Type::Uint, GL_UNSIGNED_INT },
         { Type::Float, GL_FLOAT }
      };

#define OGL_IFORMAT(f) InternalFormatLookupMap.at(f)
#define OGL_FORMAT(f) FormatLookupMap.at(f)
#define OGL_TYPE(t) TypeLookupMap.at(t)
   }
}
