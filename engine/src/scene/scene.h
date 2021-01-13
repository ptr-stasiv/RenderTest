#pragma once
#include <map>

#include "render-object.h"
#include "graphics/camera.h"
#include "graphics/light.h"
#include "graphics/material.h"

#define MAX_SCENE_OBJECTS   32
#define MAX_SCENE_LIGHTS    32
#define MAX_SCENE_MATERIALS 32

typedef struct SceneStructure
{
   RenderObject* RenderObjectList;
   int RenderObjectCount;

   Light* LightsArray;
   int LightsCount;

   PointLight* PointLightsArray;
   int PointLightsCount;

   Spotlight* SpotlightsArray;
   int SpotlightsCount;

   graphics::Material* MaterialsArray;
   int MaterialsCount;

   Camera* Camera;
} Scene;

Scene* CreateScene(Camera* camera);

void AddRenderObject(Scene* scene, const RenderObject renderObject);

void AddPhongLight(Scene* scene, const Light light);
void AddPointLight(Scene* scene, const PointLight light);
void AddSpotlight(Scene* scene, const Spotlight light);

uint8_t AddObjectMaterial(Scene* scene, const graphics::Material material);