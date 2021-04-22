#pragma once
#include <memory>
#include <cmath>

#include "graphics/render-manager.h"
#include "graphics/mesh-render.h"
#include "math/math.h"

namespace graphics
{
   //This manager will be later used to control the objects lifetime, so it isn't useless
   class DebugPrimitiveManager
   {
   private:
      std::shared_ptr<DebugPrimitiveMaterial> MaterialInstance;
      std::shared_ptr<RenderManager> RM;
   public:
      DebugPrimitiveManager(const std::shared_ptr<RenderManager>& rm)
         : RM(rm)
      {
         MaterialInstance = std::make_shared<DebugPrimitiveMaterial>();
      }

      void AddAACube(const mm::vec3& color,
                     const mm::vec3& center, const mm::vec3& size)
      {
         Mesh mesh;
         
         mesh.Vertices.Positions.emplace_back(center);
         mesh.Vertices.Positions.emplace_back(center.x, center.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y, center.z);
         mesh.Vertices.Positions.emplace_back(center);
                                 
         mesh.Vertices.Positions.emplace_back(center.x, center.y + size.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x, center.y + size.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center + size);
         mesh.Vertices.Positions.emplace_back(center + size);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y + size.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x, center.y + size.y, center.z);
                                 
         mesh.Vertices.Positions.emplace_back(center.x, center.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x, center.y + size.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y + size.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y + size.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y, center.z);
         mesh.Vertices.Positions.emplace_back(center.x, center.y, center.z);
                                 
         mesh.Vertices.Positions.emplace_back(center.x, center.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center.x, center.y + size.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center + size);
         mesh.Vertices.Positions.emplace_back(center + size);
         mesh.Vertices.Positions.emplace_back(center.x + size.x, center.y, center.z + size.z);
         mesh.Vertices.Positions.emplace_back(center.x, center.y, center.z + size.z);

         auto& material = std::make_shared<DebugPrimitiveMaterial>(*MaterialInstance);

         material->Color = color;

         RenderKey key;
         key.Depth = 0;
         key.MaterialId = material->GetId();
         key.Opaque = 1;
         key.Layer = graphics::Layer::Debug;

         mesh.Material = material;

         RM->PushRenderRequest(key, mesh);
      }

      void AddAASphere(const mm::vec3& color, const uint8_t sections,
                       const mm::vec3& center, const float radius)
      {
         ASSERT(sections % 6 == 0, "Sphere sections must be divisible by 6");

         Mesh mesh;

         float angleStep = mm::TAU / sections;

         for (size_t i = 0; i <= sections; ++i)
         {
            mesh.Vertices.Positions.emplace_back(center.x + radius * cos(angleStep * i),
                                                 center.y + radius * sin(angleStep * i),
                                                 center.z);

            if (i % 2 == 0
                && i != 0
                && i != sections)
            {
               mesh.Vertices.Positions.emplace_back(center.x + radius * cos(angleStep * i),
                                                    center.y + radius * sin(angleStep * i),
                                                    center.z);
            }
         }

         for (size_t i = 0; i <= sections; ++i)
         {
            mesh.Vertices.Positions.emplace_back(center.x + radius * cos(angleStep * i),
                                                 center.y,
                                                 center.z + radius * sin(angleStep * i));

            if (i % 2 == 0
                && i != 0
                && i != sections)
            {
               mesh.Vertices.Positions.emplace_back(center.x + radius * cos(angleStep * i),
                                                    center.y,
                                                    center.z + radius * sin(angleStep * i));
            }
         }

         for (size_t i = 0; i <= sections; ++i)
         {
            mesh.Vertices.Positions.emplace_back(center.x,
                                                 center.y + radius * cos(angleStep * i),
                                                 center.z + radius * sin(angleStep * i));

            if (i % 2 == 0
               && i != 0
               && i != sections)
            {
               mesh.Vertices.Positions.emplace_back(center.x,
                                                    center.y + radius * cos(angleStep * i),
                                                    center.z + radius * sin(angleStep * i));
            }
         }

         auto& material = std::make_shared<DebugPrimitiveMaterial>(*MaterialInstance);

         material->Color = color;

         RenderKey key;
         key.Depth = 0;
         key.MaterialId = material->GetId();
         key.Opaque = 1;
         key.Layer = graphics::Layer::Debug;

         mesh.Material = material;

         RM->PushRenderRequest(key, mesh);
      }
   };
}
