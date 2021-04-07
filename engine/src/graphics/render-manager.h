#pragma once
#include <vector>
#include <unordered_map>

#include "mesh-render.h"

#include "graphics/camera/camera.h"
#include "graphics/light/lights.h"

#include "graphics/api/devices/graphics-device.h"
#include "graphics/api/vertex-buffer.h"
#include "graphics/api/uniform-buffer.h"

namespace graphics
{
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

   inline constexpr size_t MaxVerticesPerDraw = 500'000;

   inline constexpr size_t MaxPointLights = 32;
   inline constexpr size_t MaxSpotlights = 32;

   class RenderManager
   {
   private:
      std::vector<Mesh> MeshList;

      PointLightA PointLightList[MaxPointLights];
      SpotlightA SpotlightList[MaxSpotlights];

      size_t PointLightCounter = 0;
      size_t SpotlightCounter = 0;

      std::shared_ptr<GraphicsDevice> GD;
   public:
      std::shared_ptr<VertexBuffer> PositionsVBO;
      std::shared_ptr<VertexBuffer> NormalsVBO;
      std::shared_ptr<VertexBuffer> UVsVBO;
      std::shared_ptr<VertexBuffer> TangentsVBO;

      std::shared_ptr<UniformBuffer> LightsUBO;

      RenderManager(const std::shared_ptr<GraphicsDevice>& gd);

      void Update(const Camera& camera);

      void AddMesh(const Mesh& mesh)
      {
         MeshList.push_back(mesh);
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
   };
}