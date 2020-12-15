#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

Scene* CreateScene(Camera* camera)
{
   Scene* newScene = (Scene*)malloc(sizeof(Scene));
   if (!newScene)
      return NULL;

   newScene->RenderObjectList = malloc(sizeof(RenderObject) * 32);
   newScene->RenderObjectCount = 0;

   newScene->Camera = camera;

   return newScene;
}

void AddRenderObject(Scene* scene, RenderObject* renderObject)
{
   if (scene->RenderObjectCount >= MAX_SCENE_OBJECTS)
      return;

   scene->RenderObjectList[scene->RenderObjectCount] = renderObject;
   scene->RenderObjectCount++;
}