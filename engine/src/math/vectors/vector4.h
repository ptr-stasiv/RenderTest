#pragma once
#include <cmath>

#include "vector3.h"

namespace math
{
   struct Vector4
   {
      float x, y, z, w;

      inline Vector4(const float _x, const float _y, const float _z, const float _w)
         : x(_x), y(_y), z(_z), w(_w) {}
      inline Vector4(const Vector3& v)
         : Vector4(v.x, v.y, v.z, 0.0f) {}
      inline Vector4(const float s)
         : x(s), y(s), z(s), w(s) {}
      inline Vector4()
         : Vector4(0.0f) {}

      ~Vector4() = default;

      inline Vector4 operator + (const Vector4& b)
      {
         Vector4 res;

         res.x = x + b.x;
         res.y = y + b.y;
         res.z = z + b.z;
         res.w = w + b.w;

         return res;
      }

      inline Vector4 operator - (const Vector4& b)
      {
         Vector4 res;

         res.x = x - b.x;
         res.y = y - b.y;
         res.z = z - b.z;
         res.w = w - b.w;

         return res;
      }

      inline Vector4 operator * (const float s)
      {
         Vector4 res;

         res.x = x * s;
         res.y = y * s;
         res.z = z * s;
         res.w = w * s;

         return res;
      }

      inline void operator += (const Vector4& b)
      {
         *this = *this + b;
      }

      inline void operator -= (const Vector4& b)
      {
         *this = *this - b;
      }

      inline void operator *= (const float s)
      {
         *this = *this * s;
      }
   };

   inline float Dot(const Vector4& a, const Vector4& b)
   {
      return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
   }

   inline float Length(const Vector4& v)
   {
      return sqrt(Dot(v, v));
   }

   inline Vector4 Normalize(const Vector4& v)
   {
      float d = Length(v);
      return { v.x / d, v.y / d, v.z / d, v.w / d };
   }
}
