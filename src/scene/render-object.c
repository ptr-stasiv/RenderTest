#include "render-object.h"

#include <stdio.h>
#include <stdlib.h>

RenderObject CreateRenderObject(Mesh* mesh, Texture* texture, Matrix4 transform)
{
   RenderObject ro;

   ro.MeshData = mesh;
   ro.TextureData = texture;
   ro.Transform = transform;

   return ro;
} 