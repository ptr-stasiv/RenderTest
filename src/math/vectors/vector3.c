#include "vector3.h"

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

float dot_vector3(const Vector3 a, const Vector3 b)
{
   return a.x * b.x + a.y * b.y + a.z * b.z;
}
