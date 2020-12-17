#pragma once
#include <stdint.h>
#include "../math/vectors/vector3.h"

typedef struct MeshDataStructure
{
   Vector3* Positions;
   Vector3* Normals;

   uint32_t FacesCount;
} MeshData;

MeshData LoadMesh(const char* filepath);