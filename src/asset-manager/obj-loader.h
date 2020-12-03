#pragma once
#include <stdint.h>
#include "../math/vectors/vector3.h"

typedef struct MeshDataStructure
{
   Vector3* Positions;
   uint32_t PositionsCount;
} MeshData;

MeshData LoadMesh(const char* filepath);