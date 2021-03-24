#pragma once
#include <vector>

#include "math/vectors/vector3.h"
#include "math/vectors/vector2.h"

namespace graphics
{
   struct Mesh
   {
      std::vector<math::Vector3> Positions;
      std::vector<math::Vector3> Normals;
      std::vector<math::Vector2> UVs;
      std::vector<math::Vector3> Tangents;
      std::vector<math::Vector3> Bitangents;

      inline Mesh(const std::vector<math::Vector3>& positions,
                  const std::vector<math::Vector3>& normals = { 0 },
                  const std::vector<math::Vector2>& uvs = { 0 },
                  const std::vector<math::Vector3>& tangents = { 0 },
                  const std::vector<math::Vector3>& bitangents = { 0 })
               : Positions(positions), Normals(normals), UVs(uvs),
                 Tangents(tangents), Bitangents(bitangents) {}
   };
}
