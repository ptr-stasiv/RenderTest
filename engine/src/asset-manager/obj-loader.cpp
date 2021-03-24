#include "obj-loader.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <functional>

#include "utils/timer.h"

namespace assets
{
   MeshAssetData LoadMesh(const std::string_view filepath)
   {
      MeshAssetData resData;
      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = false;

      utils::Timer loadTimer(true);

      FILE* file = fopen(&filepath[0], "rb");
      if (!file)
         return resData;

      std::vector<math::Vector3> positionArray;
      std::vector<math::Vector3> normalArray;
      std::vector<math::Vector2> uvArray;

      std::vector<uint32_t> positionIndicesArray;
      std::vector<uint32_t> normalIndicesArray;
      std::vector<uint32_t> uvIndicesArray;

      while (1)
      {
         char header[128];
         if (fscanf(file, "%s", header) == EOF)
            break;

         if (!strcmp(header, "v"))
         {
            math::Vector3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);

            positionArray.emplace_back(v);
         }
         else if (!strcmp(header, "vn"))
         {
            math::Vector3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);

            normalArray.emplace_back(v);
         }
         else if (!strcmp(header, "vt"))
         {
            math::Vector2 v;
            fscanf(file, "%f %f", &v.x, &v.y);

            uvArray.emplace_back(v);
         }
         else if (!strcmp(header, "f"))
         {
            int p1, p2, p3;
            int n1, n2, n3;
            int t1, t2, t3;

            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);

            positionIndicesArray.emplace_back(p1);
            positionIndicesArray.emplace_back(p2);
            positionIndicesArray.emplace_back(p3);

            normalIndicesArray.emplace_back(n1);
            normalIndicesArray.emplace_back(n2);
            normalIndicesArray.emplace_back(n3);

            uvIndicesArray.emplace_back(t1);
            uvIndicesArray.emplace_back(t2);
            uvIndicesArray.emplace_back(t3);
         }
      }

      std::vector<math::Vector3> resultPositionArray;
      std::vector<math::Vector3> resultNormalArray;
      std::vector<math::Vector2> resultUvArray;

      for (size_t i = 0; i < positionIndicesArray.size(); ++i)
      {
         resultPositionArray.emplace_back(positionArray[positionIndicesArray[i] - 1]);
         resultNormalArray.emplace_back(normalArray[normalIndicesArray[i] - 1]);
         resultUvArray.emplace_back(uvArray[uvIndicesArray[i] - 1]);
      }


      std::vector<math::Vector3> tangentArray;
      std::vector<math::Vector3> bitangentArray;

      for (size_t i = 0; i < resultPositionArray.size(); i += 3)
      {
         math::Vector3 p0 = resultPositionArray[i];
         math::Vector3 p1 = resultPositionArray[i + 1];
         math::Vector3 p2 = resultPositionArray[i + 2];

         math::Vector2 uv0 = resultUvArray[i];
         math::Vector2 uv1 = resultUvArray[i + 1];
         math::Vector2 uv2 = resultUvArray[i + 2];

         math::Vector3 edge1 = p1 - p0;
         math::Vector3 edge2 = p2 - p0;

         math::Vector2 uvEdge1 = uv1 - uv0;
         math::Vector2 uvEdge2 = uv2 - uv0;


         //Find matrix inverse

         float inverseD = 1 / (uvEdge1.x * uvEdge2.y - uvEdge1.y * uvEdge2.x);

         float m00 = uvEdge2.y * inverseD;
         float m01 = -uvEdge1.y * inverseD;
         float m10 = -uvEdge2.x * inverseD;
         float m11 = uvEdge1.x * inverseD;


         math::Vector3 tangent;
         tangent.x = m00 * edge1.x + m01 * edge2.x;
         tangent.y = m00 * edge1.y + m01 * edge2.y;
         tangent.z = m00 * edge1.z + m01 * edge2.z;
         tangent = math::Normalize(tangent);

         math::Vector3 bitangent;
         bitangent.x = m10 * edge1.x + m11 * edge2.x;
         bitangent.y = m10 * edge1.y + m11 * edge2.y;
         bitangent.z = m10 * edge1.z + m11 * edge2.z;
         bitangent = math::Normalize(bitangent);


         tangentArray.push_back(tangent);
         tangentArray.push_back(tangent);
         tangentArray.push_back(tangent);

         bitangentArray.push_back(bitangent);
         bitangentArray.push_back(bitangent);
         bitangentArray.push_back(bitangent);
      }


      resData.FacesCount = positionIndicesArray.size();
      resData.Positions = resultPositionArray;
      resData.Normals = resultNormalArray;
      resData.UVs = resultUvArray;
      resData.Tangents = tangentArray;
      resData.Bitangents = bitangentArray;

      resData.Info.Name     = filepath;
      resData.Info.HashedName = std::hash<std::string_view>{}(filepath);
      resData.Info.LoadTime = loadTimer.GetElapsedTime();
      resData.Info.IsValid  = true;

      return  resData;
   }
}