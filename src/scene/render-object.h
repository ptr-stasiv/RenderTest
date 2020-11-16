#pragma once
#include "mesh.h"
#include "texture.h"

typedef struct RenderSceneObjectStructure
{
   Mesh* MeshData;
   Texture* TextureData;
} RenderObject;

RenderObject* CreateRenderObject(Mesh* mesh, Texture* texture);