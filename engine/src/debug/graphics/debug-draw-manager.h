#pragma once
#include <vector>
#include <memory>

#include "math/vectors/vector3.h"
#include "graphics/shaders/shader-program.h"
#include "graphics/camera/camera.h"

namespace debug
{
   class DebugDrawManager
   {
   private:
      graphics::ShaderProgram PrimitiveShader;

      struct DrawInfo;
      std::vector<std::unique_ptr<DrawInfo>> DrawPrimitivesArray;
   public:
      DebugDrawManager();
      ~DebugDrawManager();

      void Draw(const graphics::Camera& camera);

      void AddDebugSphere(const math::Vector3& center, const float radius, const uint8_t sectors, const uint8_t stacks);
   };
}
