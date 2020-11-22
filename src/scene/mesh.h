#pragma once
#include "GL/glew.h"
#include "../math/matrices/matrix4x4.h"

typedef struct MeshDataStructure
{
   GLuint VaoId;
   int VerticesCount;
} Mesh;

Mesh* CreateMesh(float* vertices, const int verticesCount);