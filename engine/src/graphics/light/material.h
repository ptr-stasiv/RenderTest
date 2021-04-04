#pragma once
#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"

#include "asset-manager/asset-manager.h"

namespace graphics
{
   struct Material
   {
      math::Vector4 DiffuseColor;

      math::Vector3 SpecularColor;

      math::Vector3 Emissive;

      float Glossiness;

      assets::PixelsData DiffuseTexture;
      assets::PixelsData SpecularTexture;
      assets::PixelsData NormalTexture;
      assets::PixelsData EmissiveTexture;
      assets::PixelsData GlossinessTexture;
   };
}
