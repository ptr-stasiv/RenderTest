#include "obj-loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace assets
{
   MeshAssetData LoadMesh(const std::string_view filepath)
   {
      MeshAssetData resData(nullptr, nullptr, nullptr, 0);
      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = false;

      FILE* file = fopen(&filepath[0], "rb");
      if (!file)
         return resData;

      uint32_t positionCount = 0;
      uint32_t normalCount = 0;
      uint32_t uvCount = 0;

      uint32_t facesCount = 0;

      while (1)
      {
         char header[128] = { 0 };
         if (fscanf(file, "%s", header) == EOF)
            break;

         if (!strcmp(header, "v"))
            ++positionCount;
         else if (!strcmp(header, "vn"))
            ++normalCount;
         else if (!strcmp(header, "vt"))
            ++uvCount;
         else if (!strcmp(header, "f"))
            ++facesCount;
      }

      rewind(file);


      Vector3* positionArray = (Vector3*)malloc(sizeof(Vector3) * positionCount);
      uint32_t positionCounter = 0;

      Vector3* normalArray = (Vector3*)malloc(sizeof(Vector3) * normalCount);
      uint32_t normalCounter = 0;

      Vector2* uvArray = (Vector2*)malloc(sizeof(Vector2) * uvCount);
      uint32_t uvCounter = 0;


      uint32_t* positionIndicesArray = (uint32_t*)malloc(sizeof(uint32_t) * 3 * facesCount);
      uint32_t positionIndicesCounter = 0;

      uint32_t* normalIndicesArray = (uint32_t*)malloc(sizeof(uint32_t) * 3 * facesCount);
      uint32_t normalIndicesCounter = 0;

      uint32_t* uvIndicesArray = (uint32_t*)malloc(sizeof(uint32_t) * 3 * facesCount);
      uint32_t uvIndicesCounter = 0;

      while (1)
      {
         char header[128];
         if (fscanf(file, "%s", header) == EOF)
            break;

         if (!strcmp(header, "v"))
         {
            Vector3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);

            positionArray[positionCounter] = v;
            ++positionCounter;
         }
         else if (!strcmp(header, "vn"))
         {
            Vector3 v;
            fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);

            normalArray[normalCounter] = v;
            ++normalCounter;
         }
         else if (!strcmp(header, "vt"))
         {
            Vector2 v;
            fscanf(file, "%f %f", &v.x, &v.y);

            uvArray[uvCounter] = v;
            ++uvCounter;
         }
         else if (!strcmp(header, "f"))
         {
            int p1, p2, p3;
            int n1, n2, n3;
            int t1, t2, t3;

            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);

            positionIndicesArray[positionIndicesCounter] = p1;
            positionIndicesArray[positionIndicesCounter + 1] = p2;
            positionIndicesArray[positionIndicesCounter + 2] = p3;

            normalIndicesArray[normalIndicesCounter] = n1;
            normalIndicesArray[normalIndicesCounter + 1] = n2;
            normalIndicesArray[normalIndicesCounter + 2] = n3;

            uvIndicesArray[uvIndicesCounter] = t1;
            uvIndicesArray[uvIndicesCounter + 1] = t2;
            uvIndicesArray[uvIndicesCounter + 2] = t3;

            positionIndicesCounter += 3;
            normalIndicesCounter += 3;
            uvIndicesCounter += 3;
         }
      }

      Vector3* resultPositionArray = (Vector3*)malloc(sizeof(Vector3) * facesCount * 3);
      Vector3* resultNormalArray = (Vector3*)malloc(sizeof(Vector3) * facesCount * 3);
      Vector2* resultUvArray = (Vector2*)malloc(sizeof(Vector2) * facesCount * 3);

      for (int i = 0; i < facesCount * 3; ++i)
      {
         resultPositionArray[i] = positionArray[positionIndicesArray[i] - 1];
         resultNormalArray[i] = normalArray[normalIndicesArray[i] - 1];
         resultUvArray[i] = uvArray[uvIndicesArray[i] - 1];
      }

      free(positionArray);
      free(normalArray);
      free(uvArray);
      free(positionIndicesArray);
      free(normalIndicesArray);
      free(uvIndicesArray);


      resData.FacesCount = facesCount * 3;
      resData.Positions = resultPositionArray;
      resData.Normals = resultNormalArray;
      resData.UVs = resultUvArray;

      resData.Info.LoadTime = 0.0f;
      resData.Info.IsValid  = true;

      return  resData;
   }
}