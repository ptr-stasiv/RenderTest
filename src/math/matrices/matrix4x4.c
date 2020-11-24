#include "matrix4x4.h"

#include <stdio.h>
#include <math.h>

Matrix4 CreateIdentityMatrix4()
{
   Matrix4 res = { 0 };

   res.Data[0]  = 1.0f;
   res.Data[5]  = 1.0f;
   res.Data[10] = 1.0f;
   res.Data[15] = 1.0f;

   return res;
}

Matrix4 CreateTranslateMatrix(const Vector3 offset)
{
   Matrix4 res = CreateIdentityMatrix4();

   res.Data[12]  = offset.x;
   res.Data[13]  = offset.y;
   res.Data[14] = offset.z;

   return res;
}

Matrix4 CreateScaleMatrix(const Vector3 scale)
{
   Matrix4 res = { 0 };

   res.Data[3]  = scale.x;
   res.Data[7]  = scale.y;
   res.Data[11] = scale.z;
   res.Data[15] = 1.0f;

   return res;
}

Matrix4 CreateRotationMatrix(const float a, const Vector3 axis)
{
   float c = cos(a);
   float s = sin(a);

   Matrix4 zAxis = CreateIdentityMatrix4();
   zAxis.Data[0] = c;
   zAxis.Data[1] = -s;
   zAxis.Data[4] = s;
   zAxis.Data[5] = c;

   Matrix4 yAxis = CreateIdentityMatrix4();
   zAxis.Data[0]  = c;
   zAxis.Data[2]  = -s;
   zAxis.Data[8]  = s;
   zAxis.Data[10] = c;

   Matrix4 xAxis = CreateIdentityMatrix4();
   xAxis.Data[5]  = c;
   xAxis.Data[6]  = -s;
   xAxis.Data[9]  = s;
   xAxis.Data[10] = c;

   Matrix4 res = multiply_matrix(multiply_matrix(xAxis, yAxis), zAxis);
   return res;
}

Vector4 multiply_vmatrix4(const Vector4 v, const Matrix4 m)
{
   Vector4 res;

   res.x = dot_vector4(v, (Vector4){ m.Data[0], m.Data[1], m.Data[2], m.Data[3] });
   res.y = dot_vector4(v, (Vector4){ m.Data[4], m.Data[5], m.Data[6], m.Data[7] });
   res.z = dot_vector4(v, (Vector4){ m.Data[8], m.Data[9], m.Data[10], m.Data[11] });
   res.w = dot_vector4(v, (Vector4){ m.Data[12], m.Data[13], m.Data[14], m.Data[15] });

   return res;
}

Matrix4 multiply_matrix(const Matrix4 m1, const Matrix4 m2)
{
   Matrix4 res;
   
   for(int row = 0; row < 12; row += 4)
   {
      for(int column = 0; column < 4; ++column)
      {
         Vector4 m1Vec = (Vector4){ m1.Data[row], m1.Data[row + 1], m1.Data[row + 2], m1.Data[row + 3] };
         Vector4 m2Vec = (Vector4){ m2.Data[0 + column], m2.Data[1 * 4 + column], m2.Data[2 * 4 + column], m2.Data[3 * 4 + column] };
         float e = dot_vector4(m1Vec, m2Vec);

         res.Data[row + column] = e;
      }
   }

   return res;
}
