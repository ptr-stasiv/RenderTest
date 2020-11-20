#include "vector4.h"

Vector4 add_vector4(const Vector4 a, const Vector4 b)
{
   Vector4 res;

   res.x = a.x + b.x;
   res.y = a.y + b.y;
   res.z = a.z + b.z;
   res.w = a.w + b.w;

   return res;
}

Vector4 subtract_vector4(const Vector4 a, const Vector4 b)
{
   Vector4 res;

   res.x = a.x - b.x;
   res.y = a.y - b.y;
   res.z = a.z - b.z;
   res.w = a.w - b.w;

   return res;
}

Vector4 muliply_svector4(const Vector4 v, const float s)
{
   Vector4 res;

   res.x = v.x * s;
   res.y = v.y * s;
   res.z = v.z * s;
   res.w = v.w * s;

   return res;
}

float dot_vector4(const Vector4 a, const Vector4 b)
{
   return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
