#pragma once
#include "GL/glew.h"
#include "asset-manager/obj-loader.h"
#include "asset-manager/asset-manager.h"
#include "math/matrices/matrix4x4.h"

struct Mesh
{
   GLuint VaoId;
   int VerticesCount;
};

Mesh* CreateMesh(const assets::AssetRef assetRef);