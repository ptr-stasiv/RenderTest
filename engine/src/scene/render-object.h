#pragma once
#include "mesh.h"

typedef struct RenderSceneObjectStructure
{
   Mesh* MeshData;
   Matrix4 Transform;
   uint8_t MaterialRef;
} RenderObject;

RenderObject CreateRenderObject(Mesh* mesh, Matrix4 transform, const uint8_t materialRef);