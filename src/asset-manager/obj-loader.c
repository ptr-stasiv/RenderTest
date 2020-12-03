#include "obj-loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

MeshData LoadMesh(const char* filepath)
{
   FILE* file = fopen(filepath, "rb");
   if (!file)
      return (MeshData){ 0 }; 

   uint32_t positionCount = 0;
   uint32_t facesCount = 0;

   while (1)
   {
      char header[128] = { 0 };
      if (fscanf(file, "%s", header) == EOF)
         break;

      if (!strcmp(header, "v"))
         ++positionCount;
      else if (!strcmp(header, "f"))
         ++facesCount;
   }

   rewind(file);


   Vector3* positionArray = malloc(sizeof(Vector3) * positionCount);
   uint32_t positionCounter = 0;

   uint32_t* positionIndicesArray = malloc(sizeof(uint32_t) * 3 * facesCount);
   uint32_t positionIndicesCounter = 0;

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
      else if (!strcmp(header, "f"))
      {
         int p1, p2, p3;
         int n1, n2, n3;
         int t1, t2, t3;

         fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &p1, &n1, &t1, &p2, &n2, &t2, &p3, &n3, &t3);

         positionIndicesArray[positionIndicesCounter]     = p1;
         positionIndicesArray[positionIndicesCounter + 1] = p2;
         positionIndicesArray[positionIndicesCounter + 2] = p3;

         positionIndicesCounter += 3;
      }
   }

   Vector3* resultPositionArray = malloc(sizeof(Vector3) * facesCount * 3);

   for (int i = 0; i < facesCount * 3; ++i)
      resultPositionArray[i] = positionArray[positionIndicesArray[i] - 1];

   free(positionArray);
   free(positionIndicesArray);

   return  (MeshData){ resultPositionArray, facesCount * 3 };
}