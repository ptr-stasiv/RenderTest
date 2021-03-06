#pragma once

namespace math
{
   static constexpr float Pi = 3.14f;
   static constexpr float Pi2 = 2 * Pi;

   inline constexpr float ToRad(const float degree)
   {
      return degree * Pi / 180.0f;
   }
}