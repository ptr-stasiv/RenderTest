#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

Scene* CreateScene(Camera* camera)
{
   Scene* newScene = (Scene*)malloc(sizeof(Scene));
   if (!newScene)
      return NULL;

   newScene->RenderObjectList = malloc(sizeof(RenderObject) * MAX_SCENE_OBJECTS);
   newScene->RenderObjectCount = 0;

   newScene->LightsArray = malloc(sizeof(Light) * MAX_SCENE_LIGHTS);
   newScene->LightsCount = 0;

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

void AddPhongLight(Scene* scene, const Light light)
{
   if (scene->LightsCount >= MAX_SCENE_LIGHTS)
      return;

   scene->LightsArray[scene->LightsCount] = light;
   scene->LightsCount++;
}