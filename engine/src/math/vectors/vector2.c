#include "vector2.h"

#include <math.h>

Vector2 vec2(const float x, const float y)
{
   return (Vector2){ x, y };
}

Vector2 svec2(const float s)
{
   return (Vector2){ s };
}

Vector2 add_Vector2(const Vector2 a, const Vector2 b)
{
   Vector2 res;

   res.x = a.x + b.x;
   res.y = a.y + b.y;

   return res;
}

Vector2 subtract_Vector2(const Vector2 a, const Vector2 b)
{
   Vector2 res;

   res.x = a.x - b.x;
   res.y = a.y - b.y;

   return res;
}

Vector2 muliply_sVector2(const Vector2 v, const float s)
{
   Vector2 res;

   res.x = v.x * s;
   res.y = v.y * s;

   return res;
}

Vector2 normalize_Vector2(const Vector2 v)
{
   float d = length_Vector2(v);
   return (Vector2){ v.x / d, v.y / d };
}

float length_Vector2(const Vector2 v)
{
   return sqrt(dot_Vector2(v, v));
}

float dot_Vector2(const Vector2 a, const Vector2 b)
{
   return a.x * b.x + a.y * b.y;
}
