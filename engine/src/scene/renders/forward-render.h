#pragma once
#include <vector>
#include <unordered_map>

#include "../scene.h"
#include "graphics/camera.h"

namespace graphics
{
   class ForwardRender
   {
   private:
      static inline std::unordered_map<uint16_t, GLuint> LoadedTextureLookup;

   public:
      static void Initialize();

      static void Update(const core::Scene& scene, const float deltaTime);
   };
}