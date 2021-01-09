#include "vector3.h"

#include <math.h>

Vector3 vec3(const float x, const float y, const float z)
{
   return { x, y, z };
}

Vector3 svec3(const float s)
{
   return { s };
}

Vector3 add_vector3(const Vector3 a, const Vector3 b)
{
   Vector3 res;

   res.x = a.x + b.x;
   res.y = a.y + b.y;
   res.z = a.z + b.z;

   return res;
}

Vector3 subtract_vector3(const Vector3 a, const Vector3 b)
{
   Vector3 res;

   res.x = a.x - b.x;
   res.y = a.y - b.y;
   res.z = a.z - b.z;

   return res;
}

Vector3 muliply_svector3(const Vector3 v, const float s)
{
   Vector3 res;

   res.x = v.x * s;
   res.y = v.y * s;
   res.z = v.z * s;

   return res;
}

Vector3 normalize_vector3(const Vector3 v)
{
   float d = length_vector3(v);
   return { v.x / d, v.y / d, v.z / d };
}

Vector3 cross_vector3(const Vector3 a, const Vector3 b)
{
   return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

float length_vector3(const Vector3 v)
{
   return sqrt(dot_vector3(v, v));
}

float dot_vector3(const Vector3 a, const Vector3 b)
{
   return a.x * b.x + a.y * b.y + a.z * b.z;
}
