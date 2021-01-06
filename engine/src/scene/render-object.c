#include "render-object.h"

#include <stdio.h>
#include <stdlib.h>

RenderObject CreateRenderObject(Mesh* mesh, Texture* texture, Matrix4 transform, const uint8_t materialRef)
{
   return (RenderObject) { mesh, texture, transform, materialRef };
} 