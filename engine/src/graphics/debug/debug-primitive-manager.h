#pragma once
#include <memory>

#include "graphics/render-manager.h"
#include "graphics/mesh-render.h"
#include "math/vectors/vector.h"

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

      void AddAACube(const math::Vector3& center, const math::Vector3& size)
      {
         Mesh mesh;
         
      }

      void AddAASphere(const math::Vector3& center, const float radius)
      {

      }
   };
}
