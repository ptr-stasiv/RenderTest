#pragma once
#include "math/math.h"

namespace graphics
{
   struct PointLight
   {
      mm::vec3 Position;
      mm::vec3 Color;

      float Stretch;
      float Offset;

      PointLight() = default;

      inline PointLight(const mm::vec3 position,
                        const mm::vec3 color,
                        const float stretch,
                        const float offset)
         : Position(position),
           Color(color),
           Stretch(stretch),
           Offset(offset) {}
   };

   struct Spotlight
   {
      mm::vec3 Position;
      mm::vec3 Direction;
      mm::vec3 Color;

      float InnerAngle;
      float OuterAngle;

      Spotlight() = default;

      inline Spotlight(const mm::vec3& position,
                       const mm::vec3& direction,
                       const mm::vec3& color,
                       const float outerAngle,
                       const float innerAngle)
         : Position(position),
           Direction(direction),
           Color(color),
           OuterAngle(outerAngle),
           InnerAngle(innerAngle) {}
   };
}
