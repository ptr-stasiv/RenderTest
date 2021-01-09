#include "scene.h"

#include <stdio.h>
#include <stdlib.h>

Scene* CreateScene(Camera* camera)
{
   Scene* newScene = (Scene*)malloc(sizeof(Scene));
   if (!newScene)
      return NULL;

   newScene->RenderObjectList = (RenderObject*)malloc(sizeof(RenderObject) * MAX_SCENE_OBJECTS);
   newScene->RenderObjectCount = 0;

   newScene->LightsArray = (Light*)malloc(sizeof(Light) * MAX_SCENE_LIGHTS);
   newScene->LightsCount = 0;

   newScene->PointLightsArray = (PointLight*)malloc(sizeof(PointLight) * MAX_SCENE_LIGHTS);
   newScene->PointLightsCount = 0;

   newScene->SpotlightsArray = (Spotlight*)malloc(sizeof(Spotlight) * MAX_SCENE_LIGHTS);
   newScene->SpotlightsCount = 0;

   newScene->MaterialsArray = (Material*)malloc(sizeof(Material) * MAX_SCENE_MATERIALS);
   newScene->MaterialsCount = 0;

   newScene->Camera = camera;

   return newScene;
}

void AddRenderObject(Scene* scene, const RenderObject renderObject)
{
   if (scene->RenderObjectCount > MAX_SCENE_OBJECTS)
      return;

   scene->RenderObjectList[scene->RenderObjectCount++] = renderObject;
}

void AddPhongLight(Scene* scene, const Light light)
{
   if (scene->LightsCount > MAX_SCENE_LIGHTS)
      return;

   scene->LightsArray[scene->LightsCount++] = light;
}

void AddPointLight(Scene* scene, const PointLight light)
{
   if (scene->PointLightsCount > MAX_SCENE_LIGHTS)
      return;

   scene->PointLightsArray[scene->PointLightsCount++] = light;
}

void AddSpotlight(Scene* scene, const Spotlight light)
{
   if (scene->SpotlightsCount > MAX_SCENE_LIGHTS)
      return;

   scene->SpotlightsArray[scene->SpotlightsCount++] = light;
}

uint8_t AddObjectMaterial(Scene* scene, const Material material)
{
   if (scene->MaterialsCount > MAX_SCENE_MATERIALS)
      return -1;

   scene->MaterialsArray[scene->MaterialsCount] = material;
   return scene->MaterialsCount++;
}