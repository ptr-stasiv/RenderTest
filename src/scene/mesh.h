#pragma once
#include "GL/glew.h"
#include "../math/matrices/matrix4x4.h"

typedef struct MeshStructure
{
   GLuint VaoId;
   int VerticesCount;
} Mesh;

Mesh* CreateMesh(Vector3* vertices, const int verticesCount);