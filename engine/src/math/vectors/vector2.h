#pragma once
#include <cmath>

namespace math
{
   struct Vector2
   {
      float x, y;

      inline Vector2(const float _x, const float _y)
         : x(_x), y(_y) {}
      inline Vector2(const float s)
         : x(s), y(s) {}

      Vector2() = default;
      ~Vector2() = default;

      inline Vector2 operator + (const Vector2& b)
      {
         Vector2 res;

         res.x = x + b.x;
         res.y = y + b.y;

         return res;
      }

      inline Vector2 operator - (const Vector2& b)
      {
         Vector2 res;

         res.x = x - b.x;
         res.y = y - b.y;

         return res;
      }

      inline Vector2 operator * (const float s)
      {
         Vector2 res;

         res.x = x * s;
         res.y = y * s;

         return res;
      }

      inline void operator += (const Vector2& b)
      {
         *this = *this + b;
      }

      inline void operator -= (const Vector2& b)
      {
         *this = *this - b;
      }

      inline void operator *= (const float s)
      {
         *this = *this * s;
      }
   };

   inline float Dot(const Vector2& a, const Vector2& b)
   {
      return a.x * b.x + a.y * b.y;
   }

   inline float Length(const Vector2& v)
   {
      return sqrt(Dot(v, v));
   }

   inline Vector2 Normalize(const Vector2& v)
   {
      float d = Length(v);
      return { v.x / d, v.y / d };
   }
}