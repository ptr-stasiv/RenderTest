#pragma once
#include "GL/glew.h"
#include "../asset-manager/obj-loader.h"
#include "../math/matrices/matrix4x4.h"

typedef struct MeshStructure
{
   GLuint VaoId;
   int VerticesCount;
} Mesh;

Mesh* CreateMesh(const MeshData meshData);