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
      std::vector<math::Vector3> Positions;
      std::vector<math::Vector3> Normals;
      std::vector<math::Vector2> UVs;
      std::vector<math::Vector3> Tangents;
      std::vector<math::Vector3> Bitangents;

      size_t FacesCount;

      inline MeshAssetData(const std::vector<math::Vector3>& positions, const std::vector<math::Vector3>& normals, const std::vector<math::Vector2>& uvs, 
                           const std::vector<math::Vector3>& tangents, const std::vector<math::Vector3>& bitangents, const size_t faces)
         : Positions(positions), Normals(normals), UVs(uvs), Tangents(tangents), Bitangents(bitangents), FacesCount(faces) {}

      inline MeshAssetData() = default;

      ASSET_TYPE(AssetType::Mesh)
   };

   MeshAssetData LoadMesh(const std::string_view filepath);
}