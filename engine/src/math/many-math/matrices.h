#pragma once
#include "math/many-math/vectors.h"

namespace mm
{
   template<typename T, size_t Rows, size_t Columns>
   struct n_mat;

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
      };
   };

   template<typename T>
   struct n_mat<T, 4, 3>
   {
      T Data[4 * 3];
   };

   template<typename T>
   struct n_mat<T, 3, 3>
   {
      T Data[3 * 3];
   };

   template<typename T>
   struct n_mat<T, 2, 2>
   {
      T Data[2 * 2];
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

            for (size_t i = 0.0f; i < Columns; ++i)
               elem += mat1.Data[i + r * Rows] * mat2.Data[c + i * Columns2];

            res.Data[r + c * Columns2] = elem;
         }
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

   template<typename T, size_t Rows>
   inline n_mat<T, Rows, 4> translate(const mm::n_vector<T, Rows>& offset)
   {
      
   }

   template<typename T, size_t Rows, size_t Columns>
   inline n_mat<T, Rows, Columns> scale(const mm::n_vector<T, Rows>& scaleFactors)
   {

   }


   inline n_mat<float, 4, 4> perspective(const float fov, const float ar, const float nearZ, const float farZ)
   {
      n_mat<float, 4, 4> res{ 0 };

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
      n_mat<float, 4, 4> res{ 0 };

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
}
