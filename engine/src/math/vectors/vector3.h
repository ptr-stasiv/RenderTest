#pragma once
#include <cmath>

namespace math
{
   struct Vector3
   {
      float x, y, z;

      inline Vector3(const float _x, const float _y, const float _z)
         : x(_x), y(_y), z(_z) {}
      inline Vector3(const float s)
         : x(s), y(s), z(s) {}

      Vector3()  = default;
      ~Vector3() = default;

      inline Vector3 operator + (const Vector3& b)
      {
         Vector3 res;

         res.x = x + b.x;
         res.y = y + b.y;
         res.z = z + b.z;

         return res;
      }

      inline Vector3 operator - (const Vector3& b)
      {
         Vector3 res;

         res.x = x - b.x;
         res.y = y - b.y;
         res.z = z - b.z;

         return res;
      }

      inline Vector3 operator * (const float s)
      {
         Vector3 res;

         res.x = x * s;
         res.y = y * s;
         res.z = z * s;

         return res;
      }

      inline void operator += (const Vector3& b)
      {
         *this = *this + b;
      }

      inline void operator -= (const Vector3& b)
      {
         *this = *this - b;
      }

      inline void operator *= (const float s)
      {
         *this = *this * s;
      }
   };

   inline Vector3 Cross(const Vector3& a, const Vector3& b)
   {
      return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
   }

   inline float Dot(const Vector3& a, const Vector3& b)
   {
      return a.x * b.x + a.y * b.y + a.z * b.z;
   }

   inline float Length(const Vector3& v)
   {
      return sqrt(Dot(v, v));
   }

   inline Vector3 Normalize(const Vector3& v)
   {
      float d = Length(v);
      return { v.x / d, v.y / d, v.z / d };
   }
}
