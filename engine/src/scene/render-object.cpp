#include "render-object.h"
#include "math/matrices/matrix4x4.h"

#include <stdio.h>
#include <stdlib.h>

RenderObject CreateRenderObject(Mesh* mesh,  Matrix4 transform, const uint8_t materialRef)
{
   return { mesh, transform, materialRef };
} 