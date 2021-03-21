#pragma once
#include <vector>
#include <unordered_map>

#include "graphics/renderer.h"

#include "graphics/camera/camera.h"
#include "graphics/light/lights.h"

#include "graphics/api/devices/graphics-device.h"
#include "graphics/api/vertex-buffer.h"
#include "graphics/api/uniform-buffer.h"

#include "asset-manager/image-loader.h"

namespace graphics
{
   inline constexpr size_t MaxVerticesPerDraw = 500'000;

   inline constexpr size_t MaxPointLights = 32;
   inline constexpr size_t MaxSpotlights = 32;


   //Shader values

   inline constexpr uint8_t PositionAttribLocation = 0;
   inline constexpr uint8_t NormalAttribLocation = 1;
   inline constexpr uint8_t UvAttribLocation = 2;


   struct alignas(16) MaterialA
   {
      math::Vector4 DiffuseColor;

      math::Vector4 SpecularColor;

      math::Vector4 Emissive;

      float Glossiness;
   };

   struct alignas(16) PointLightA
   {
      math::Vector4 Position;
      math::Vector4 Color;
                              
      float Stretch;
      float Offset;
   };

   struct alignas(16) SpotlightA
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
      
      PointLightA PointLightList[MaxPointLights];
      SpotlightA SpotlightList[MaxSpotlights]; 

      size_t PointLightCounter = 0;
      size_t SpotlightCounter = 0;

      std::shared_ptr<VertexBuffer> PositionsVBO;
      std::shared_ptr<VertexBuffer> NormalsVBO;
      std::shared_ptr<VertexBuffer> UVsVBO;

      std::shared_ptr<UniformBuffer> LightUBO;
      std::shared_ptr<UniformBuffer> MaterialUBO;

      std::unordered_map<size_t, std::shared_ptr<graphics::Texture2D>> TextureLookup;

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
         PointLightA uboPL;
         uboPL.Position = pl.Position;
         uboPL.Color = pl.Color;
         uboPL.Stretch = pl.Stretch;
         uboPL.Offset = pl.Offset;

         PointLightList[PointLightCounter++] = uboPL;
      }

      inline void AddLight(const Spotlight& sl)
      {
         SpotlightA uboSL;
         uboSL.Position = sl.Position;
         uboSL.Direction = sl.Direction;
         uboSL.Color = sl.Color;
         uboSL.InnerAngle = sl.InnerAngle;
         uboSL.OuterAngle = sl.OuterAngle;

         SpotlightList[SpotlightCounter++] = uboSL;
      }
   private:
      void UpdateLight();
      void ResolveTextures(const Material& material);
   };
}
