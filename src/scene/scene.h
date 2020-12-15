#pragma once
#include "render-object.h"
#include "../graphics/camera.h"

#define MAX_SCENE_OBJECTS 32

typedef struct SceneStructure
{
   RenderObject** RenderObjectList;
   int RenderObjectCount;

   Camera* Camera;
} Scene;

Scene* CreateScene(Camera* camera);

void AddRenderObject(Scene* scene, RenderObject* renderObject);