#pragma once
#include "mesh.h"
#include "texture.h"

typedef struct RenderSceneObjectStructure
{
   Mesh* MeshData;
   Texture* TextureData;
   Matrix4 Transform;
   uint8_t MaterialRef;
} RenderObject;

RenderObject CreateRenderObject(Mesh* mesh, Texture* texture, Matrix4 transform, const uint8_t materialRef);