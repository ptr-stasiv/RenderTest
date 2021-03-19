#pragma once
#include <vector>

#include "graphics/renderer.h"

#include "graphics/camera/camera.h"
#include "graphics/light/lights.h"

#include "graphics/api/devices/graphics-device.h"
#include "graphics/api/vertex-buffer.h"
#include "graphics/api/uniform-buffer.h"

namespace graphics
{
   inline constexpr size_t MaxVerticesPerDraw = 500'000;

   inline constexpr size_t MaxPointLights = 32;
   inline constexpr size_t MaxSpotlights = 32;


   //Shader values

   inline constexpr uint8_t PositionAttribLocation = 0;
   inline constexpr uint8_t NormalAttribLocation = 1;
   inline constexpr uint8_t UvAttribLocation = 2;


   struct alignas(16) MaterialUBO
   {
      math::Vector4 DiffuseColor;

      math::Vector4 SpecularColor;

      math::Vector4 Emissive;
   };

   struct alignas(16) PointLightUBO
   {
      math::Vector4 Position;
      math::Vector4 Color;
                              
      float Quadratic;
      float Linear;
      float Constant;
   };

   struct alignas(16) SpotlightUBO
   {
      math::Vector4 Position;
      math::Vector4 Direction;
      math::Vector4 Color;
                              
      float InnerAngle;
      float OuterAngle;
   };


   class ForwardRender
   {
   private:
      std::shared_ptr<ShaderProgram> MainShader;

      std::vector<Renderer> RendererList;
      
      PointLightUBO PointLightList[MaxPointLights];
      SpotlightUBO SpotlightList[MaxSpotlights]; 

      size_t PointLightCounter = 0;
      size_t SpotlightCounter = 0;

      std::shared_ptr<VertexBuffer> PositionsVBO;
      std::shared_ptr<VertexBuffer> NormalsVBO;
      std::shared_ptr<VertexBuffer> UVsVBO;

      std::shared_ptr<UniformBuffer> LightUBO;

      std::shared_ptr<graphics::GraphicsDevice> GraphicsDevice;
   public:
      ForwardRender(const std::shared_ptr<graphics::GraphicsDevice>& device);

      void Render(const Camera& camera); //This is temporary solution for camera

      inline void AddRenderer(const Renderer& r)
      {
         RendererList.push_back(r);
      }

      inline void AddLight(const PointLight& pl)
      {
         PointLightUBO uboPL;
         uboPL.Position = pl.Position;
         uboPL.Color = pl.Color;
         uboPL.Quadratic = pl.Quadratic;
         uboPL.Linear = pl.Linear;
         uboPL.Constant = pl.Constant;

         PointLightList[PointLightCounter++] = uboPL;
      }

      inline void AddLight(const Spotlight& sl)
      {
         SpotlightUBO uboSL;
         uboSL.Position = sl.Position;
         uboSL.Direction = sl.Direction;
         uboSL.Color = sl.Color;
         uboSL.InnerAngle = sl.InnerAngle;
         uboSL.OuterAngle = sl.OuterAngle;

         SpotlightList[SpotlightCounter] = uboSL;
      }
   private:
      void UpdateLight();
   };
}
