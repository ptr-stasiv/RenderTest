#include "matrix4x4.h"

#include <stdio.h>

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

   res.Data[3]  = offset.x;
   res.Data[7]  = offset.y;
   res.Data[11] = offset.z;

   return res;
}

Matrix4 CreateScaleMatrix(const Vector3 scale)
{

}

Matrix4 CreateRotationMatrix(const float a, const Vector3 axis)
{

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
