#include "render-object.h"

#include <stdio.h>
#include <stdlib.h>

RenderObject* CreateRenderObject(Mesh* mesh, Texture* texture)
{
   RenderObject* res = malloc(sizeof(RenderObject));
   if (!res)
      return NULL;

   res->MeshData = mesh;
   res->TextureData = texture;

   return res;
} 