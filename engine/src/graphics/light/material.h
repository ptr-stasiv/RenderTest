#pragma once
#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"
#include "asset-manager/image-loader.h"

namespace graphics
{
   struct Material
   {
      math::Vector4 DiffuseColor;

      math::Vector3 SpecularColor;

      math::Vector3 Emissive;

      float Glossiness;

      assets::ImageAssetData DiffuseTexture;
      assets::ImageAssetData SpecularTexture;
      assets::ImageAssetData NormalTexture;
      assets::ImageAssetData EmissiveTexture;
      assets::ImageAssetData GlossinessTexture;
   };
}
