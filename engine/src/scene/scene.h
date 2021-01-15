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
   class Scene
   {
   public:
      std::deque<graphics::RenderObject> RenderObjectsList;

      std::deque<graphics::Light> LightsList;
      std::deque<graphics::PointLight> PointLightsList;
      std::deque<graphics::Spotlight> SpotlightsList;

      std::deque<graphics::Material> MaterialsList;

      std::reference_wrapper<graphics::Camera> CameraRef;

      inline Scene(graphics::Camera& camera)
         : CameraRef(camera) {}

      inline void AddRenderObject(const graphics::RenderObject& ro)
      {
         RenderObjectsList.emplace_back(ro);
      }

      inline void AddLight(const graphics::Light& light)
      {
         LightsList.emplace_back(light);
      }

      inline void AddLight(const graphics::PointLight& light)
      {
         PointLightsList.emplace_back(light);
      }

      inline void AddLight(const graphics::Spotlight& light)
      {
         SpotlightsList.emplace_back(light);
      }

      inline size_t AddMaterial(const graphics::Material& material)
      {
         MaterialsList.emplace_back(material);
         return MaterialsList.size() - 1;
      }

      inline void SetCamera(graphics::Camera& camera) { CameraRef = camera; }
      inline graphics::Camera GetCamera() const { return CameraRef.get(); }
   };
}