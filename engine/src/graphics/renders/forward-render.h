#pragma once
#include <vector>

#include "graphics/renderer.h"

#include "graphics/camera/camera.h"
#include "graphics/light/lights.h"

#include "graphics/api/devices/graphics-device.h"
#include "graphics/api/vertex-buffer.h"

namespace graphics
{
   constexpr size_t MaxVerticesPerDraw = 500'000;

   //Each shader that that must be used with this render must have the same layout
   constexpr uint8_t PositionAttribLocation = 0;
   constexpr uint8_t NormalAttribLocation = 1;
   constexpr uint8_t UvAttribLocation = 2;


   class ForwardRender
   {
   private:
      std::shared_ptr<ShaderProgram> MainShader;

      std::vector<Renderer> RendererList;
      
      std::vector<PointLight> PointLightList;
      std::vector<Spotlight> SpotlightList;

      std::shared_ptr<VertexBuffer> PositionsVBO;
      std::shared_ptr<VertexBuffer> NormalsVBO;
      std::shared_ptr<VertexBuffer> UVsVBO;

      std::shared_ptr<graphics::GraphicsDevice> GraphicsDevice;
   public:
      ForwardRender(const std::shared_ptr<graphics::GraphicsDevice>& device);
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
