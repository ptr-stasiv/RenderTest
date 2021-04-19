#include "matrix4x4.h"

#include <cstdio>
#include <cmath>

namespace math
{
   Matrix4 CreateIdentityMatrix4()
   {
      Matrix4 res = { 0 };

      res.Data[0] = 1.0f;
      res.Data[5] = 1.0f;
      res.Data[10] = 1.0f;
      res.Data[15] = 1.0f;

      return res;
   }

   Matrix4 CreatePerspectiveMatrix(const float aspect, const float fov, const float near, const float far)
   {
      Matrix4 res = { 0 };

      res.Data[0] = 1.0f / (aspect * tan(fov / 2.0f));
      res.Data[5] = 1.0f / tan(fov / 2.0f);
      res.Data[10] = -((far + near) / (far - near));
      res.Data[11] = -1.0f;
      res.Data[14] = (-2 * far * near) / (far - near);

      return res;
   }

   Matrix4 CreateTranslateMatrix(const mm::vec3& offset)
   {
      Matrix4 res = CreateIdentityMatrix4();

      res.Data[12] = offset.x;
      res.Data[13] = offset.y;
      res.Data[14] = offset.z;

      return res;
   }

   Matrix4 CreateScaleMatrix(const mm::vec3& scale)
   {
      Matrix4 res = { 0 };

      res.Data[0] = scale.x;
      res.Data[5] = scale.y;
      res.Data[10] = scale.z;
      res.Data[15] = 1.0f;

      return res;
   }

   Matrix4 CreateRotationMatrix(const float a, const mm::vec3& axis)
   {
      float c = cos(a);
      float s = sin(a);

      Matrix4 zAxis = CreateIdentityMatrix4();
      zAxis.Data[0] = c;
      zAxis.Data[1] = -s;
      zAxis.Data[4] = s;
      zAxis.Data[5] = c;

      Matrix4 yAxis = CreateIdentityMatrix4();
      zAxis.Data[0] = c;
      zAxis.Data[2] = -s;
      zAxis.Data[8] = s;
      zAxis.Data[10] = c;

      Matrix4 xAxis = CreateIdentityMatrix4();
      xAxis.Data[5] = c;
      xAxis.Data[6] = -s;
      xAxis.Data[9] = s;
      xAxis.Data[10] = c;

      Matrix4 res = (xAxis * yAxis) * zAxis;
      return res;
   }

   Matrix4 CreateLookAtMatrix(const mm::vec3& axisX, const mm::vec3& axisY, const mm::vec3& axisZ, const mm::vec3& offset)
   {
      Matrix4 res = CreateIdentityMatrix4();

      res.Data[12] = -mm::dot(axisX, offset);
      res.Data[13] = -mm::dot(axisY, offset);
      res.Data[14] = -mm::dot(axisZ, offset);

      //X axis
      res.Data[0] = axisX.x;
      res.Data[4] = axisX.y;
      res.Data[8] = axisX.z;

      //Y axis 
      res.Data[1] = axisY.x;
      res.Data[5] = axisY.y;
      res.Data[9] = axisY.z;

      //Z axis
      res.Data[2] = axisZ.x;
      res.Data[6] = axisZ.y;
      res.Data[10] = axisZ.z;

      return res;
   }

   mm::vec4 Matrix4::operator * (const mm::vec4& v)
   {
      mm::vec4 res;

      res.x = mm::dot(v, { Data[0],  Data[1],  Data[2],  Data[3] });
      res.y = mm::dot(v, { Data[4],  Data[5],  Data[6],  Data[7] });
      res.z = mm::dot(v, { Data[8],  Data[9],  Data[10], Data[11] });
      res.w = mm::dot(v, { Data[12], Data[13], Data[14], Data[15] });

      return res;
   }

   Matrix4 Matrix4::operator * (const Matrix4& m)
   {
      Matrix4 res;

      for (int row = 0; row < 16; row += 4)
      {
         mm::vec4 rowVector = { Data[row], Data[row + 1], Data[row + 2], Data[row + 3] };

         for (int column = 0; column < 4; ++column)
         {
            mm::vec4 columnVector = { m.Data[column], m.Data[column + 4], m.Data[column + 8], m.Data[column + 12] };

            float elem = mm::dot(rowVector, columnVector);
            res.Data[row + column] = elem;
         }
      }

      return res;
   }
}
