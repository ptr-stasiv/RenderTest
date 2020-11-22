#pragma once
#include "mesh.h"
#include "texture.h"

typedef struct RenderSceneObjectStructure
{
   Mesh* MeshData;
   Texture* TextureData;
   Matrix4 Transform;
} RenderObject;

RenderObject* CreateRenderObject(Mesh* mesh, Texture* texture, Matrix4 transform);