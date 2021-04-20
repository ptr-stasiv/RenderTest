#pragma once
#include <vector>

#include "entry-point/global_systems.h"

namespace scene
{
   struct Scene
   {
      std::vector<std::shared_ptr<graphics::Mesh>> RegisteredMeshes;
      std::vector<std::shared_ptr<graphics::PointLight>> RegisteredPointLights;
      std::vector<std::shared_ptr<graphics::Spotlight>> RegisteredSpotlights;

      std::shared_ptr<graphics::Camera> SceneCamera;
   };

   inline void Register(Scene& scene, const std::shared_ptr<graphics::Camera>& camera)
   {
      scene.SceneCamera = camera;
   };

   inline void Register(Scene& scene, const std::shared_ptr<graphics::Mesh>& mesh)
   {
      scene.RegisteredMeshes.push_back(mesh);
   };

   inline void Register(Scene& scene, const std::shared_ptr<graphics::PointLight>& pl)
   {
      scene.RegisteredPointLights.push_back(pl);
   }

   inline void Register(Scene& scene, const std::shared_ptr<graphics::Spotlight>& sl)
   {
      scene.RegisteredSpotlights.push_back(sl);
   }

   inline void UpdateAndRender(const Scene& scene)
   {
      for(auto& mesh : scene.RegisteredMeshes)
      {
         graphics::RenderKey rk;
         rk.Depth = ceilf(-mm::length(scene.SceneCamera->Position - mesh->Translate));
         rk.MaterialId = 0;
         rk.Opaque = 1;
         rk.Layer = graphics::Layer::Normal;

         g_RenderManager->PushRenderRequest(rk, *mesh); 
      }

      for(auto& pl : scene.RegisteredPointLights)
         g_RenderManager->PushLight(*pl);

      for(auto& sl : scene.RegisteredSpotlights)
         g_RenderManager->PushLight(*sl);

      if(scene.SceneCamera)
         g_RenderManager->Update(*scene.SceneCamera);
   }
}
