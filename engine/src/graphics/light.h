#pragma once
#include "../math/vectors/vector3.h"

namespace graphics
{
   struct Light
   {
      math::Vector3 Position;
      math::Vector3 Color;

      inline Light(const math::Vector3& position, const math::Vector3& color)
         : Position(position), Color(color) {}
   };

   struct PointLight
   {
      math::Vector3 Position;
      math::Vector3 Color;

      float Linear;
      float Constant;
      float Quadratic;

      inline PointLight(const math::Vector3& position, const math::Vector3& color, const float linear, const float constant, const float quadratic)
         : Position(position), Color(color), Linear(linear), Constant(constant), Quadratic(quadratic) {}
   };

   struct Spotlight
   {
      math::Vector3 Position;
      math::Vector3 Direction;
      math::Vector3 Color;

      float OuterAngle;
      float InnerAngle;

      inline Spotlight(const math::Vector3& position, const math::Vector3& direction, const math::Vector3& color, const float outerAngle, const float innerAngle)
         : Position(position), Direction(direction), Color(color), OuterAngle(outerAngle), InnerAngle(innerAngle) {}
   };
}