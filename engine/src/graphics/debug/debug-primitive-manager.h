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
      std::vector<Mesh> MeshList;

      std::shared_ptr<RenderManager> RM;
   public:
      DebugPrimiteManager(const std::shared_ptr<RenderManager>& rm)
         : RM(rm) {}

      void AddAACube(const mm::vec3& center, const mm::vec3& size)
      {
         Mesh mesh;
         
         //mesh.Material = new DebugPrimitiveMaterial; // Fix memory leak
         
         std::vector<mm::vec3> positions;
         positions.push_back({ 0.0f, 0.0f, 0.0f });
         positions.push_back({ 0.0f, 0.0f, 1.0f });
         positions.push_back({ 1.0f, 0.0f, 1.0f });
         positions.push_back({ 1.0f, 0.0f, 1.0f });
         positions.push_back({ 1.0f, 0.0f, 0.0f });
         positions.push_back({ 0.0f, 0.0f, 0.0f });

         mesh.Vertices.Positions = positions;

         mesh.Vertices.FacesCount = 2;

         MeshList.push_back(mesh);
      }

      void AddAASphere(const mm::vec3& center, const float radius)
      {

      }

      void Update()
      {
         for (auto& mesh : MeshList)
         {
            RenderKey key;
            key.Depth = 0;
            key.MaterialId = 0;
            key.Opaque = 1;
            key.Layer = graphics::Layer::Debug;

            RM->PushRenderRequest(key, mesh);
         }

         MeshList.clear();
      }
   };
}
