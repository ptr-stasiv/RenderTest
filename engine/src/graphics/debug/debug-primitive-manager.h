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

      void AddAACube(const mm::vec3& center, const mm::vec3& size)
      {
         Mesh mesh;
         
         mesh.Material = std::make_shared<DebugPrimitiveMaterial>();
         
         mesh.Vertices.Positions.push_back({ -1.0f, 0.0f, -1.0f });
         mesh.Vertices.Positions.push_back({ -1.0f, 0.0f, 1.0f });
         mesh.Vertices.Positions.push_back({ 1.0f, 0.0f, 1.0f });

         RenderKey key;
         key.Depth = 0;
         key.MaterialId = mesh.Material->GetId();
         key.Opaque = 1;
         key.Layer = graphics::Layer::Debug;

         RM->PushRenderRequest(key, mesh);
      }

      void AddAASphere(const mm::vec3& center, const float radius)
      {

      }
   };
}
