#include "render-object.h"

#include <stdio.h>
#include <stdlib.h>

RenderObject* CreateRenderObject(Mesh* mesh, Texture* texture, Matrix4 transform)
{
   RenderObject* res = malloc(sizeof(RenderObject));
   if (!res)
      return NULL;

   res->MeshData     = mesh;
   res->TextureData  = texture;
   res->Transform    = transform;

   return res;
} 