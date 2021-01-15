#pragma once
#include "math/vectors/vector3.h"
#include "asset-manager/image-loader.h"

namespace graphics
{
   class Material
   {
   public:
      math::Vector3 Color;
      math::Vector3 Specular;
      math::Vector3 Emissive;
      float   ShineExponent;

      assets::AssetRef DiffuseTexture;
   };
}