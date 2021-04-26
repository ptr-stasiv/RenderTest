#pragma once
#include <algorithm>

#include "math/many-math/vectors.h"

namespace mm
{
   template<typename T, size_t Rows, size_t Columns>
   struct n_mat;

   template<typename T, size_t Rows, size_t Columns>
   inline n_mat<T, Rows, Columns> identity(const n_mat<T, Rows, Columns>& mat);

#define MATRIX_CONSTRUCTOR(rows, columns) \
   n_mat() \
   { \
      *this = identity(*this); \
   } \
   n_mat(const T& scalar) \
   { \
      for(size_t i = 0; i < rows * columns; ++i) \
         Data[i] = scalar; \
   }

   template<typename T>
   struct n_mat<T, 4, 4>
   {
      union
      {
         T Data[4 * 4];

         struct
         {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
         };

         struct
         {
            mm::vec4 Row1;
            mm::vec4 Row2;
            mm::vec4 Row3;
            mm::vec4 Row4;
         };
      };

      MATRIX_CONSTRUCTOR(4, 4)
   };

   template<typename T>
   struct n_mat<T, 3, 4>
   {
      union
      {
         T Data[3 * 4];

         struct
         {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
         };

         struct 
         {
            mm::vec4 Row1;
            mm::vec4 Row2;
            mm::vec4 Row3;
         };
      };

      MATRIX_CONSTRUCTOR(3, 4)
   };

   template<typename T>
   struct n_mat<T, 2, 3>
   {
      union
      {
         T Data[2 * 3];

         struct
         {
            float m11, m12, m13;
            float m21, m22, m23;
         };

         struct
         {
            mm::vec3 Row1;
            mm::vec3 Row2;
         };
      };

      MATRIX_CONSTRUCTOR(2, 3)
   };

   template<typename T>
   struct n_mat<T, 3, 3>
   {
      union
      {
         T Data[3 * 3];

         struct 
         {
            float m11, m12, m13;
            float m21, m22, m23;
            float m31, m32, m33;
         };

         struct 
         {
            mm::vec3 Row1;
            mm::vec3 Row2;
            mm::vec3 Row3;
         };
      };

      MATRIX_CONSTRUCTOR(3, 3)
   };

   template<typename T>
   struct n_mat<T, 2, 2>
   {
      union
      {
         T Data[2 * 2];

         struct
         {
            float m11, m12;
            float m21, m22;
         };

         struct 
         {
            mm::vec2 Row1;
            mm::vec2 Row2;
         };
      };

      MATRIX_CONSTRUCTOR(2, 2)
   };


   template<typename T, size_t Rows, size_t Columns>
   mm::n_vector<T, Columns> operator * (const mm::n_vector<T, Rows>& v, const mm::n_mat<T, Rows, Columns>& mat)
   {
      mm::n_vector<T, Columns> res;

      for(size_t c = 0; c < Columns; ++c)
      {
         float elem = 0.0f;

         for(size_t r = 0; r < Rows; ++r)
            elem += v.Data[r] * mat.Data[r + c * Columns];

         res.Data[c] = elem;
      }

      return res;
   };

   template<typename T, size_t Rows, size_t Columns, size_t Columns2>
   inline n_mat<T, Rows, Columns2> operator * (const n_mat<T, Rows, Columns>& mat1, const n_mat<T, Columns, Columns2>& mat2)
   {
      n_mat<T, Rows, Columns2> res{ 0 };

      for (size_t r = 0; r < Rows; ++r)
      {
         for (size_t c = 0; c < Columns2; ++c)
         {
            float elem = 0.0f;

            for (size_t i = 0; i < Columns; ++i)
            {
               float v1 = mat1.Data[i + r * Columns];
               float v2 = mat2.Data[c + i * Columns2];

               elem += v1 * v2;
            }

            res.Data[c + r * Columns2] = elem;
         }
      }

      return res;
   }

   template<typename T, size_t Rows, size_t Columns>
   inline n_mat<T, Rows, Columns> identity(const n_mat<T, Rows, Columns>& mat)
   {
      n_mat<T, Rows, Columns> res{ 0 };

      size_t rowOffset = 0;
      for(size_t i = 0; i < Rows; ++i)
      {
         res.Data[i * Columns + rowOffset] = 1;
         ++rowOffset;
      }

      return res;
   }

   template<typename T, size_t Rows, size_t Columns>
   inline n_mat<T, Columns, Rows> transpose(const n_mat<T, Rows, Columns>& mat)
   {
      n_mat<T, Columns, Rows> res{ 0 };

      for(size_t r = 0; r < Rows; ++r)
      {
         for(size_t c = 0; c < Columns; ++c)
            res.Data[r + c * Columns] = mat.Data[c + r * Columns];
      }

      return res;
   }

   template<typename T, size_t Rows, size_t Columns, size_t VectorComponents>
   inline n_mat<T, Rows, Columns> translate(const n_mat<T, Rows, Columns>& base, const mm::n_vector<T, VectorComponents> offset)
   {
      n_mat<T, Rows, Columns> res;

      for(size_t i = 0; i < std::min(VectorComponents, Rows); ++i)
         res.Data[i * Columns + Columns - 1] = offset.Data[i];

      return base * res;
   }

   template<typename T, size_t Rows, size_t Columns, size_t VectorComponents>
   inline n_mat<T, Rows, Columns> scale(const n_mat<T, Rows, Columns>& base, const mm::n_vector<T, VectorComponents>& scaleFactors)
   {
      n_mat<T, Rows, Columns> res;

      size_t rowOffset = 0;
      for(size_t i = 0; i < std::min(VectorComponents, Rows); ++i)
      {
         res.Data[i * Columns + rowOffset] = scaleFactors.Data[i];
         ++rowOffset;
      }

      return base * res;
   }

   template<typename T, size_t Rows, size_t Columns, size_t VectorComponents>
   inline n_mat<T, Rows, Columns> rotate(const n_mat<T, Rows, Columns>& base, const float angle, const mm::n_vector<T, VectorComponents>& rotateFactors)
   {
      static_assert(Rows >= 3 && Columns >= 3);

      const float s = sin(angle);
      const float c = cos(angle);

      mm::n_mat<T, Rows, Columns> zAxisMat;
      zAxisMat.Data[0] = c * rotateFactors.z;
      zAxisMat.Data[1] = s * rotateFactors.z;
      zAxisMat.Data[4] = -s * rotateFactors.z;
      zAxisMat.Data[5] = c * rotateFactors.z;
      zAxisMat.Data[10] = 1.0f;

      mm::n_mat<T, Rows, Columns> yAxisMat;
      yAxisMat.Data[0] = c * rotateFactors.y;
      yAxisMat.Data[2] = -s * rotateFactors.y;
      yAxisMat.Data[5] = 1.0f;
      yAxisMat.Data[8] = s * rotateFactors.y;
      yAxisMat.Data[10] = c * rotateFactors.y;

      mm::n_mat<T, Rows, Columns> xAxisMat;
      xAxisMat.Data[0] = 1.0f;
      xAxisMat.Data[5] = c  * rotateFactors.x;
      xAxisMat.Data[6] = s * rotateFactors.x;
      xAxisMat.Data[10] = -s * rotateFactors.x ;
      xAxisMat.Data[11] = c * rotateFactors.x;

      return base * (xAxisMat * yAxisMat * zAxisMat);
   }

   inline n_mat<float, 4, 4> lookAt(const mm::vec3& axisX, const mm::vec3& axisY, const mm::vec3& axisZ, const mm::vec3& offset)
   {
      n_mat<float, 4, 4> res;

      res.Row1 = axisX;
      res.Row2 = axisY;
      res.Row3 = axisZ;

      res.Data[3] = -mm::dot(axisX, offset);
      res.Data[7] = -mm::dot(axisY, offset);
      res.Data[11] = -mm::dot(axisZ, offset);

      return res;
   }

   inline n_mat<float, 4, 4> perspective(const float fov, const float ar, const float nearZ, const float farZ)
   {
      n_mat<float, 4, 4> res;

      res.Data[0] = 1 / (tan(fov / 2.0f) * ar);
      res.Data[5] = 1 / tan(fov / 2.0f);

      const float b = (2 * farZ * nearZ) / (nearZ - farZ);
      const float a = (farZ - nearZ) / (nearZ - farZ);

      res.Data[10] = a;
      res.Data[11] = b;

      res.Data[14] = -1.0f;

      return res;
   }

   inline n_mat<float, 4, 4> ortho(const float right, const float top, const float farZ, const float nearZ)
   {
      n_mat<float, 4, 4> res;

      res.Data[0] = 1.0f / right;
      res.Data[5] = 1.0f / top;


      const float a = 2 / (farZ - nearZ);
      const float b = (-farZ - nearZ) / (farZ - nearZ);

      res.Data[10] = a;
      res.Data[11] = b;


      res.Data[15] = 1.0f;

      return res;
   }

   using mat4 = n_mat<float, 4, 4>; 
   using mat3 = n_mat<float, 3, 3>;
   using mat2 = n_mat<float, 2, 2>;

   using mat3x4 = n_mat<float, 3, 4>;
   using mat2x3 = n_mat<float, 2, 3>;
}
