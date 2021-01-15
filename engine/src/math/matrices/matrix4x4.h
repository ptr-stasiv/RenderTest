#pragma once
#include "../vectors/vector4.h"
#include "../vectors/vector3.h"

namespace math
{
   struct Matrix4
   {
      float Data[16];

      math::Vector4 operator * (const math::Vector4 v);

      Matrix4 operator * (const Matrix4& m);

      inline void operator *= (const Matrix4& m)
      {
         *this = *this * m;
      }
   };

   Matrix4 CreateIdentityMatrix4();
   Matrix4 CreatePerspectiveMatrix(const float aspect, const float fov, const float near, const float far);
   Matrix4 CreateTranslateMatrix(const Vector3 offset);
   Matrix4 CreateScaleMatrix(const Vector3 scale);
   Matrix4 CreateRotationMatrix(const float a, const Vector3 axis);
   Matrix4 CreateLookAtMatrix(const Vector3 axisX, const Vector3 axisY, const Vector3 axisZ, const Vector3 offset);
}