#pragma once
#include "GL/glew.h"
#include "asset-manager/obj-loader.h"
#include "asset-manager/asset-manager.h"
#include "math/matrices/matrix4x4.h"

namespace graphics
{
   struct Mesh
   {
      GLuint   VaoId;
      uint64_t VerticesCount;

      Mesh(const assets::AssetRef& assetRef);
   };
}