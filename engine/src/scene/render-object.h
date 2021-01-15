#pragma once
#include "mesh.h"

namespace graphics
{
   struct RenderObject
   {
      Mesh           MeshData;
      math::Matrix4  Transform;
      size_t         MaterialRef;

      inline RenderObject(const Mesh& meshData, const math::Matrix4& transform, size_t materialRef)
         : MeshData(meshData), Transform(transform), MaterialRef(materialRef) {}
   };
}