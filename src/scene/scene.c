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