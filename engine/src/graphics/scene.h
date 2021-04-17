#pragma once
#include <vector>

#include "entry-point/global_systems.h"

namespace scene
{
   struct Scene
   {
      std::vector<graphics::Mesh*> RegisteredMeshes;
      std::vector<graphics::PointLight*> RegisteredPointLights;
      std::vector<graphics::Spotlight*> RegisteredSpotlights;

      graphics::Camera* SceneCamera;
   };

   inline void Register(Scene& scene, graphics::Camera* camera)
   {
      scene.SceneCamera = camera;
   };

   inline void Register(Scene& scene, graphics::Mesh* mesh)
   {
      scene.RegisteredMeshes.push_back(mesh);
   };

   inline void Register(Scene& scene, graphics::PointLight* pl)
   {
      scene.RegisteredPointLights.push_back(pl);
   }

   inline void Register(Scene& scene, graphics::Spotlight* sl)
   {
      scene.RegisteredSpotlights.push_back(sl);
   }

   inline void UpdateAndRender(const Scene& scene)
   {
      for(auto& mesh : scene.RegisteredMeshes)
      {
         graphics::RenderKey rk;
         rk.Depth = ceilf(-math::Length(scene.SceneCamera->Position - mesh->Translate));
         rk.MaterialId = 0;
         rk.Opaque = 1;
         rk.Layer = graphics::Layer::Normal;

         g_RenderManager->PushRenderRequest(rk, *mesh); 
      }

      for(auto& pl : scene.RegisteredPointLights)
         g_RenderManager->PushLight(*pl);

      for(auto& sl : scene.RegisteredSpotlights)
         g_RenderManager->PushLight(*sl);

      g_RenderManager->Update(*scene.SceneCamera);
   }
}
