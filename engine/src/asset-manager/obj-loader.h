#pragma once
#include <stdint.h>
#include "../math/vectors/vector3.h"
#include "../math/vectors/vector2.h"

typedef struct MeshDataStructure
{
   Vector3* Positions;
   Vector3* Normals;
   Vector2* UVs;

   uint32_t FacesCount;
} MeshData;

MeshData LoadMesh(const char* filepath);