#pragma once
#include "render-object.h"
#include "../graphics/camera.h"
#include "../graphics/light.h"

#define MAX_SCENE_OBJECTS 32
#define MAX_SCENE_LIGHTS  32

typedef struct SceneStructure
{
   RenderObject** RenderObjectList;
   int RenderObjectCount;

   Light* LightsArray;
   int LightsCount;

   Camera* Camera;
} Scene;

Scene* CreateScene(Camera* camera);

void AddRenderObject(Scene* scene, RenderObject* renderObject);
void AddPhongLight(Scene* scene, const Light light);