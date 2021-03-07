#pragma once
#include <vector>
#include <memory>

#include "math/vectors/vector3.h"
#include "graphics/shaders/shader-pipeline.h"
#include "graphics/camera.h"

namespace debug
{
   class DebugDrawManager
   {
   private:
      graphics::ShaderPipeline PrimitiveShader;

      struct DrawInfo;
      std::vector<std::unique_ptr<DrawInfo>> DrawPrimitivesArray;
   public:
      DebugDrawManager();
      ~DebugDrawManager();

      void Draw(const graphics::Camera& camera);

      void AddDebugSphere(const math::Vector3& center, const float radius, const uint8_t sectors, const uint8_t stacks);
   };
}
