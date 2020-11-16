#pragma once
#include "render-object.h"

#define MAX_SCENE_OBJECTS 32

typedef struct SceneStructure
{
   RenderObject** RenderObjectList;
   int RenderObjectCount;
} Scene;

Scene* CreateScene();

void AddRenderObject(Scene* scene, RenderObject* renderObject);

void RenderScene(Scene* scene);