#pragma once
#include "math/vectors/vector3.h"

namespace graphics
{
   struct Material
   {
      math::Vector3 DiffuseColor;

      math::Vector3 SpecularColor;

      math::Vector3 Emissive;

      float Glossiness;

      //Texture DiffuseTexture;
      //Texture SpecularTexture;
      //Textuer NormalTexture;
      //Texture EmissiveTexture;
      //Texture GlossinessTexture;
   };
}
