#pragma once
#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"

namespace graphics
{
   struct PointLight
   {
      math::Vector3 Position;
      math::Vector3 Color;

      float Stretch;
      float Offset;

      PointLight() = default;

      inline PointLight(const math::Vector3 position,
                        const math::Vector3 color,
                        const float stretch,
                        const float offset)
         : Position(position),
           Color(color),
           Stretch(stretch),
           Offset(offset) {}
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
