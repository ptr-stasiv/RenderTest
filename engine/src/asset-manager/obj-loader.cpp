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

      for (int i = 0; i < positionIndicesArray.size(); ++i)
      {
         resultPositionArray.emplace_back(positionArray[positionIndicesArray[i] - 1]);
         resultNormalArray.emplace_back(normalArray[normalIndicesArray[i] - 1]);
         resultUvArray.emplace_back(uvArray[uvIndicesArray[i] - 1]);
      }

      resData.FacesCount = positionIndicesArray.size();
      resData.Positions = resultPositionArray;
      resData.Normals = resultNormalArray;
      resData.UVs = resultUvArray;

      resData.Info.Name     = filepath;
      resData.Info.HashedName = std::hash<std::string_view>{}(filepath);
      resData.Info.LoadTime = loadTimer.GetElapsedTime();
      resData.Info.IsValid  = true;

      return  resData;
   }
}