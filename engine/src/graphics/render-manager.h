#pragma once
#include <vector>
#include <unordered_map>
#include <functional>

#include "graphics_config.h"

#include "mesh-render.h"

#include "graphics/camera/camera.h"
#include "graphics/light/lights.h"

#include "graphics/api/devices/graphics-device.h"
#include "graphics/api/vertex-buffer.h"
#include "graphics/api/uniform-buffer.h"

namespace graphics
{
   struct RenderCfg
   {
      int32_t ShadowWidth = cfg::ShadowWidth;
      int32_t ShadowHeight = cfg::ShadowHeight;

      float ShadowBias = cfg::ShadowBias;
      float LightSize = cfg::LightSize;
      int SoftShadows = cfg::SoftShadows;
   };

   union RenderKey
   {
      uint64_t KeyId;

      struct
      {
         uint32_t Depth : 32;
         uint32_t MaterialId : 16;
         uint32_t Opaque : 1;
         uint32_t Layer : 15;
      };

      inline bool operator > (const RenderKey& rk) const
      {
         return KeyId > rk.KeyId;
      }
   };

   struct Layer
   {
      enum : uint32_t
      {
         Normal,
         Debug,
         Hud
      };
   };

   struct alignas(16) PointLightAligned16
   {
      mm::mat4 Cameras[6];

      mm::vec4 Position;
      mm::vec4 Color;

      float Stretch;
      float Offset;

      float FrustrumWidth;

      uint32_t ShadowMapId;
   };

   struct alignas(16) SpotlightAligned16
   {
      mm::mat4 Camera;

      mm::vec4 Position;
      mm::vec4 Direction;
      mm::vec4 Color;

      float InnerAngle;
      float OuterAngle;

      float FrustrumWidth;

      uint32_t ShadowMapId;
   };


   inline constexpr size_t MaxVerticesPerDraw = 500'000;

   inline constexpr size_t MaxPointLights = 32;
   inline constexpr size_t MaxSpotlights = 32;

   class RenderManager
   {
   private:
      std::shared_ptr<Texture2D> GeneralShadowMap;
      std::shared_ptr<Cubemap> GeneralCubeShadowMap;
      std::shared_ptr<Framebuffer> GeneralShadowFBO;

      mm::mat4 ortho;
      mm::mat4 view;

      std::vector<std::pair<RenderKey, Mesh>> CurrentRenderQueue;

      PointLightAligned16 PointLightList[MaxPointLights];
      SpotlightAligned16 SpotlightList[MaxSpotlights];

      size_t PointLightCounter = 0;
      size_t SpotlightCounter = 0;

      std::shared_ptr<GraphicsDevice> GD;

      void ShadowPass(const Camera& camera);
      void LightPass(const Camera& camera);
      void GeometryPass(const Camera& camera);
   public:
      std::shared_ptr<VertexBuffer> PositionsVBO;
      std::shared_ptr<VertexBuffer> NormalsVBO;
      std::shared_ptr<VertexBuffer> UVsVBO;
      std::shared_ptr<VertexBuffer> TangentsVBO;

      std::shared_ptr<UniformBuffer> LightsUBO;
      std::shared_ptr<UniformBuffer> RenderCfgUBO;

      std::shared_ptr<Texture2D> ShadowMaps[MaxSpotlights];
      std::shared_ptr<Cubemap> CubeShadowMaps[MaxPointLights];

      RenderManager(const std::shared_ptr<GraphicsDevice>& gd);

      void Update(const Camera& camera);

      inline void PushRenderRequest(const RenderKey& key, const Mesh& mesh)
      {
         CurrentRenderQueue.emplace_back(key, mesh);
      }


      //TODO move structure's reassigning to the scene setup
      
      inline void PushLight(const PointLight& pl)
      {
         PointLightAligned16 uboPL;
         uboPL.Position = pl.Position;
         uboPL.Color = pl.Color;
         uboPL.Stretch = pl.Stretch;
         uboPL.Offset = pl.Offset;
         uboPL.ShadowMapId = SpotlightCounter;

         PointLightList[PointLightCounter++] = uboPL;
      }

      inline void PushLight(const Spotlight& sl)
      {
         SpotlightAligned16 uboSL;
         uboSL.Position = sl.Position;
         uboSL.Direction = sl.Direction;
         uboSL.Color = sl.Color;
         uboSL.InnerAngle = sl.InnerAngle;
         uboSL.OuterAngle = sl.OuterAngle;
         uboSL.ShadowMapId = SpotlightCounter;

         SpotlightList[SpotlightCounter++] = uboSL;
      }

      inline size_t GetPointLightsCount() const
      {
         return PointLightCounter;
      }

      inline size_t GetSpotlightsCount() const
      {
         return SpotlightCounter;
      }
   };
}