#pragma once
#include "math/vectors/vector3.h"
#include "asset-manager/image-loader.h"

namespace graphics
{
   class Material
   {
   public:
      Vector3 Color;
      Vector3 Specular;
      Vector3 Emissive;
      float   ShineExponent;

      assets::AssetRef DiffuseTexture;
   };
}