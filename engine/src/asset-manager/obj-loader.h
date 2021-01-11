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
      Vector3* Positions;
      Vector3* Normals;
      Vector2* UVs;

      uint32_t FacesCount;

      inline MeshAssetData(Vector3* positions, Vector3* normals, Vector2* uvs, const uint32_t faces)
         : Positions(positions), Normals(normals), UVs(uvs), FacesCount(faces) {}

      ASSET_TYPE(AssetType::Mesh)
   };

   MeshAssetData LoadMesh(const char* filepath);
}