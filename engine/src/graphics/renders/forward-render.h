#pragma once
#include <vector>

#include "platform-render.h"
#include "graphics/renderer.h"

#include "graphics/camera/camera.h"
#include "graphics/light/lights.h"

namespace graphics
{
   class ForwardRender
   {
   private:
      std::unique_ptr<ShaderProgram> MainShader;

      std::vector<Renderer> RendererList;
      std::unique_ptr<PlatformRender> CurrentRender;
      
      std::vector<PointLight> PointLightList;
      std::vector<Spotlight> SpotlightList;
   public:
      ForwardRender();
      ~ForwardRender();

      void Render(const Camera& camera); //This is temporary solution for camera

      inline void AddRenderer(const Renderer& r)
      {
         RendererList.push_back(r);
      }

      inline void AddLight(const PointLight& pl)
      {
         PointLightList.push_back(pl);
      }

      inline void AddLight(const Spotlight& sl)
      {
         SpotlightList.push_back(sl);
      }
   };
}
