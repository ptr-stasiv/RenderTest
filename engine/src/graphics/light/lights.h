#pragma once
#include "math/vectors/vector3.h"
#include "math/vectors/vector4.h"

namespace graphics
{
   //Layouts of the structures shouldn't be changed without careful check it's can break render!!!

   struct alignas(16) PointLight
   {
      math::Vector4 Position;
      math::Vector4 Color;

      float Quadratic;
      float Linear;
      float Constant;

      inline PointLight(const math::Vector4 position,
                        const math::Vector4 color,
                        const float quadratic,
                        const float linear,
                        const float constant)
         : Position(position),
           Color(color),
           Quadratic(quadratic),
           Linear(linear),
           Constant(constant) {}                 
   };

   struct alignas(16) Spotlight
   {
      math::Vector4 Position;
      math::Vector4 Direction;
      math::Vector4 Color;

      float InnerAngle;
      float OuterAngle;

      inline Spotlight(const math::Vector4& position,
                       const math::Vector4& direction,
                       const math::Vector4& color,
                       const float outerAngle,
                       const float innerAngle)
         : Position(position),
           Direction(direction),
           Color(color),
           OuterAngle(outerAngle),
           InnerAngle(innerAngle) {}
   };
}
