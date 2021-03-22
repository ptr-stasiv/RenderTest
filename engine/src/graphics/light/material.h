#pragma once
#include <memory>

#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"
#include "asset-manager/image-loader.h"
#include "graphics/api/texture2d.h"

namespace graphics
{
   struct Material
   {
      math::Vector4 DiffuseColor;

      math::Vector3 SpecularColor;

      math::Vector3 Emissive;

      float Glossiness;

      std::shared_ptr<graphics::Texture2D> DiffuseTexture;
      std::shared_ptr<graphics::Texture2D> SpecularTexture;
      std::shared_ptr<graphics::Texture2D> NormalTexture;
      std::shared_ptr<graphics::Texture2D> EmissiveTexture;
      std::shared_ptr<graphics::Texture2D> GlossinessTexture;
   };
}
