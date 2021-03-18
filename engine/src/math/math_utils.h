#pragma once

namespace math
{
   inline constexpr float Pi = 3.14f;
   inline constexpr float Pi2 = 2.0f * Pi;

   inline constexpr float ToRad(const float degree)
   {
      return degree * Pi / 180.0f;
   }
}