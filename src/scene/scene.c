#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

Scene* CreateScene()
{
   Scene* res = (Scene*)malloc(sizeof(Scene));
   if (!res)
      return NULL;

   res->RenderObjectList = malloc(sizeof(RenderObject) * 32);
   res->RenderObjectCount = 0;

   return res;
}

void AddRenderObject(Scene* scene, RenderObject* renderObject)
{
   if (scene->RenderObjectCount >= MAX_SCENE_OBJECTS)
      return;

   scene->RenderObjectList[scene->RenderObjectCount] = renderObject;
   scene->RenderObjectCount++;
}

void RenderScene(Scene* scene)
{
   for (int i = 0; i < scene->RenderObjectCount; ++i)
   {
      RenderObject* r = scene->RenderObjectList[i];

      glBindVertexArray(r->MeshData->VaoId);

      glDrawArrays(GL_TRIANGLES, 0, r->MeshData->VerticesCount);

      glBindVertexArray(0);
   }
}