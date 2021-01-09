#include "render-object.h"

#include <stdio.h>
#include <stdlib.h>

RenderObject CreateRenderObject(Mesh* mesh, Texture* texture, Matrix4 transform, const uint8_t materialRef)
{
   return { mesh, texture, transform, materialRef };
} 