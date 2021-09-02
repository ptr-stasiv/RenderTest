#pragma once
#include <unordered_map>

#include "GL/glew.h"
#include "graphics/api/api.h"

namespace graphics
{
   namespace gl
   {
      inline std::unordered_map<Feature, GLenum> FeatureLookupMap = 
      {
         { Feature::Blend, GL_BLEND },
         { Feature::Depth, GL_DEPTH_TEST },
         { Feature::SRGB, GL_FRAMEBUFFER_SRGB }
      };

      inline std::unordered_map<Attachment, GLenum> AttachmentLookupMap =
      {
         { Attachment::Color, GL_COLOR_ATTACHMENT0 },
         { Attachment::Depth, GL_DEPTH_ATTACHMENT }
      };

      inline std::unordered_map<InternalFormat, GLenum> InternalFormatLookupMap =
      {
         { InternalFormat::RGB, GL_RGB },
         { InternalFormat::RGB8, GL_RGB8 },
         { InternalFormat::RGBA, GL_RGBA },
         { InternalFormat::RGBA8, GL_RGBA8 },
         { InternalFormat::RGB16, GL_RGB16 },
         { InternalFormat::RGBA16, GL_RGBA16 },
         { InternalFormat::Depth16, GL_DEPTH_COMPONENT16 },
         { InternalFormat::Depth24, GL_DEPTH_COMPONENT24 },
         { InternalFormat::Depth32, GL_DEPTH_COMPONENT32 },
         { InternalFormat::RGB32F, GL_RGB32F },
         { InternalFormat::RGBA32F, GL_RGBA32F },
         { InternalFormat::SRGB8, GL_SRGB8 }
      };

      inline std::unordered_map<Format, GLenum> FormatLookupMap =
      {
         { Format::RGB, GL_RGB },
         { Format::RGBA, GL_RGBA },
         { Format::BGR, GL_BGR },
         { Format::BGRA, GL_BGRA },
         { Format::Depth, GL_DEPTH_COMPONENT }
      };

      inline std::unordered_map<Type, GLenum> TypeLookupMap =
      {
         { Type::Ubyte, GL_UNSIGNED_BYTE },
         { Type::Uint, GL_UNSIGNED_INT },
         { Type::Float, GL_FLOAT }
      };

      inline std::unordered_map<BlendFunc, GLenum> BlendFuncLookupMap = 
      {
         { BlendFunc::Add, GL_FUNC_ADD },
         { BlendFunc::Subtract, GL_FUNC_SUBTRACT }
      };

      inline std::unordered_map<BlendValue, GLenum> BlendValueLookupMap = 
      {  
         { BlendValue::One, GL_ONE },
         { BlendValue::SrcAlpha, GL_SRC_ALPHA },
         { BlendValue::OneMinusSrcAlpha, GL_ONE_MINUS_SRC_ALPHA }
      };

      inline std::unordered_map<Face, GLenum> FaceValueLookupMap =
      {
         { Face::Front, GL_FRONT },
         { Face::Back, GL_BACK },
         { Face::Both, GL_FRONT_AND_BACK }
      };
   }
}

#define OGL_FEATURE(f) graphics::gl::FeatureLookupMap.at(f)
#define OGL_ATTACHMENT(a) graphics::gl::AttachmentLookupMap.at(a)
#define OGL_IFORMAT(f) graphics::gl::InternalFormatLookupMap.at(f)
#define OGL_FORMAT(f) graphics::gl::FormatLookupMap.at(f)
#define OGL_TYPE(t) graphics::gl::TypeLookupMap.at(t)
#define OGL_BLEND_FUNC(f) graphics::gl::BlendFuncLookupMap.at(f)
#define OGL_BLEND_VALUE(v) graphics::gl::BlendValueLookupMap.at(v)
#define OGL_FACE(f) graphics::gl::FaceValueLookupMap.at(f)
