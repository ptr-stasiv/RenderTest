#pragma once
#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"

namespace graphics
{
   struct PointLight
   {
      math::Vector3 Position;
      math::Vector3 Color;

      float Quadratic;
      float Linear;
      float Constant;

      PointLight() = default;

      inline PointLight(const math::Vector3 position,
                        const math::Vector3 color,
                        const float quadratic,
                        const float linear,
                        const float constant)
         : Position(position),
           Color(color),
           Quadratic(quadratic),
           Linear(linear),
           Constant(constant) {}                 
   };

   struct Spotlight
   {
      math::Vector3 Position;
      math::Vector3 Direction;
      math::Vector3 Color;

      float InnerAngle;
      float OuterAngle;

      Spotlight() = default;

      inline Spotlight(const math::Vector3& position,
                       const math::Vector3& direction,
                       const math::Vector3& color,
                       const float outerAngle,
                       const float innerAngle)
         : Position(position),
           Direction(direction),
           Color(color),
           OuterAngle(outerAngle),
           InnerAngle(innerAngle) {}
   };
}
