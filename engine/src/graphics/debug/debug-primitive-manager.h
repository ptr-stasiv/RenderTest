#pragma once
#include <memory>

#include "graphics/render-manager.h"
#include "graphics/mesh-render.h"
#include "math/math.h"

namespace graphics
{
   //This manager will be later used to control the objects lifetime, so it isn't useless
   class DebugPrimiteManager
   {
   private:
      std::shared_ptr<RenderManager> RM;
   public:
      DebugPrimiteManager(const std::shared_ptr<RenderManager>& rm)
         : RM(rm) {}

      void AddAACube(const mm::vec4& color,
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

         auto material = std::make_shared<DebugPrimitiveMaterial>();

         material->Color = color;

         RenderKey key;
         key.Depth = 0;
         key.MaterialId = material->GetId();
         key.Opaque = 1;
         key.Layer = graphics::Layer::Debug;

         mesh.Material = material;

         RM->PushRenderRequest(key, mesh);
      }

      void AddAASphere(const mm::vec3& center, const float radius)
      {

      }
   };
}
