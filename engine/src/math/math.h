#pragma once
#include "many-math/vectors.h"

namespace mm
{
   constexpr float PI = 3.14159265359f;
   constexpr float TAU = 6.283185307179586;

   constexpr float ToRad(const float degrees)
   {
      return PI * degrees / 180.0f;
   }
}