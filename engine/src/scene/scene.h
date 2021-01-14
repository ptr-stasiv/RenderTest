#pragma once
#include <deque>
#include <memory>
#include <functional>

#include "render-object.h"
#include "graphics/camera.h"
#include "graphics/light.h"
#include "graphics/material.h"

namespace core
{
   //class Scene
   //{
   //public:
   //   std::deque<RenderObject> RenderObjectsList;

   //   std::deque<Light>      LightsList;
   //   std::deque<PointLight> PointLightsList;
   //   std::deque<Spotlight>  SpotlightsList;

   //   std::deque<graphics::Material> MaterialsList;

   //   std::shared_ptr<Camera> MainCamera;

   //   inline Scene(const std::shared_ptr<Camera>& camera)
   //      : MainCamera(camera) {}

   //   Scene() = default;
   //   ~Scene() = default;

   //   inline void AddRenderObject(const RenderObject& ro)
   //   {
   //      RenderObjectsList.emplace_back(ro);
   //   }

   //   inline void AddLight(const Light& light)
   //   {
   //      LightsList.emplace_back(light);
   //   }

   //   inline void AddLight(const PointLight& light)
   //   {
   //      PointLightsList.emplace_back(light);
   //   }

   //   inline void AddLight(const Spotlight& light)
   //   {
   //      SpotlightsList.emplace_back(light);
   //   }

   //   inline size_t AddMaterial(const graphics::Material& material)
   //   {
   //      MaterialsList.emplace_back(material);
   //      return MaterialsList.size() - 1;
   //   }
   //};
   

   class Scene
   {
   public:
      std::deque<RenderObject> RenderObjectsList;

      std::deque<Light> LightsList;
      std::deque<PointLight> PointLightsList;
      std::deque<Spotlight> SpotlightsList;

      std::deque<graphics::Material> MaterialsList;

      std::reference_wrapper<Camera> CameraRef;

      inline Scene(Camera& camera)
         : CameraRef(camera) {}

      inline void AddRenderObject(const RenderObject& ro)
      {
         RenderObjectsList.emplace_back(ro);
      }

      inline void AddLight(const Light& light)
      {
         LightsList.emplace_back(light);
      }

      inline void AddLight(const PointLight& light)
      {
         PointLightsList.emplace_back(light);
      }

      inline void AddLight(const Spotlight& light)
      {
         SpotlightsList.emplace_back(light);
      }

      inline size_t AddMaterial(const graphics::Material& material)
      {
         MaterialsList.emplace_back(material);
         return MaterialsList.size() - 1;
      }

      inline void SetCamera(Camera& camera) { CameraRef = camera; }
      inline Camera GetCamera() const { return CameraRef.get(); }
   };
}