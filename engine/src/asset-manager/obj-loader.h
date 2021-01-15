#pragma once
#include <stdint.h>

#include "../math/vectors/vector3.h"
#include "../math/vectors/vector2.h"

#include "asset-manager.h"

namespace assets
{
   class MeshAssetData : public AssetData
   {
   public:
      math::Vector3* Positions;
      math::Vector3* Normals;
      math::Vector2* UVs;

      uint32_t FacesCount;

      inline MeshAssetData(math::Vector3* positions, math::Vector3* normals, math::Vector2* uvs, const uint32_t faces)
         : Positions(positions), Normals(normals), UVs(uvs), FacesCount(faces) {}

      ASSET_TYPE(AssetType::Mesh)
   };

   MeshAssetData LoadMesh(const std::string_view filepath);
}