#pragma once
#include "GL/glew.h"

typedef struct MeshDataStructure
{
   GLuint VaoId;
   int VerticesCount;
} Mesh;

Mesh* CreateMesh(float* vertices, const int verticesCount);