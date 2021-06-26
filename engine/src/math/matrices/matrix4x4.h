#pragma once
#include "math/math.h"

namespace math
{
   struct Matrix4
   {
      float Data[16];

      mm::vec4 operator * (const mm::vec4& v);

      Matrix4 operator * (const Matrix4& m);

      inline void operator *= (const Matrix4& m)
      {
         *this = *this * m;
      }
   };
}