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

   Matrix4 CreateIdentityMatrix4();
   Matrix4 CreatePerspectiveMatrix(const float aspect, const float fov, const float near, const float far);
   Matrix4 CreateTranslateMatrix(const mm::vec3& offset);
   Matrix4 CreateScaleMatrix(const mm::vec3& scale);
   Matrix4 CreateRotationMatrix(const float a, const mm::vec3& axis);
   Matrix4 CreateLookAtMatrix(const mm::vec3& axisX, const mm::vec3& axisY, const mm::vec3& axisZ, const mm::vec3& offset);
}