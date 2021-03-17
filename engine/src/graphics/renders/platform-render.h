#pragma once
#include <memory>

#include "graphics/mesh.h"
#include "graphics/light/material.h"

namespace graphics
{
   constexpr size_t MaxVerticesPerDraw = 500'000;

   //Each shader that that must be used with this render must have the same layout
   constexpr uint8_t PositionAttribLocation = 0;
   constexpr uint8_t NormalAttribLocation = 1;
   constexpr uint8_t UvAttribLocation = 2;

   //Wrapper for basic render features
   //Render mesh and supply to shader attributes of position, normal and uv

   struct PlatformRender
   {
      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;

      PlatformRender();
      ~PlatformRender();

      PlatformRender(PlatformRender&&) = default;
      PlatformRender& operator = (PlatformRender&&) = default;
   };

   std::unique_ptr<PlatformRender> CreatePlatformRender();

   void DestroyPlatformRender(const std::unique_ptr<PlatformRender>& pr);

   //Before calling this funtion shader must be used
   void SetPlatformRenderMaterial(const std::unique_ptr<PlatformRender>& pr, const Material& material);

   void DrawPlatformRender(const std::unique_ptr<PlatformRender>& pr, const Mesh& mesh);
   //void BatchDrawPlatformRender(const std::unique_ptr<PlatformRender>& pr, const std::vector<Mesh>& meshList);
}
