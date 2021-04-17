#pragma once
#include <cstdint>
#include <cstring>

namespace mm
{
   template<typename T, size_t Components>
   struct n_vector
   {
      T Data[Components];
   };

   template<typename T, size_t Components, size_t ResultComponents, size_t ...ComponentsPermutation>
   struct n_swizzle
   {
      T AliasedData[Components];

      operator n_vector<T, ResultComponents> ()
      {
         T resData[ResultComponents] = { AliasedData[ComponentsPermutation]... };

         n_vector<T, ResultComponents> resVector;

         memcpy_s(resVector.Data, sizeof(T) * ResultComponents,
                  resData, sizeof(T) * ResultComponents);

         return resVector;
      }
   };

   template<typename T>
   struct n_vector<T, 2>
   {
      union
      {
         T Data[2];

         float x;
         float y;

         n_swizzle<T, 2, 2, 0, 1> xy;
         n_swizzle<T, 2, 2, 1, 0> yx;
      };
   };

   template<typename T>
   struct n_vector<T, 3>
   {
      union 
      {
         T Data[3];

         float x;
         float y;
         float z;

         n_swizzle<T, 3, 2, 0, 1> xy;
         n_swizzle<T, 3, 2, 1, 0> yx;
         n_swizzle<T, 3, 2, 0, 2> xz;
         n_swizzle<T, 3, 2, 2, 0> zx;
         n_swizzle<T, 3, 2, 1, 2> yz;
         n_swizzle<T, 3, 2, 2, 1> zy;
         
         n_swizzle<T, 3, 3, 0, 1, 2> xyz;
         n_swizzle<T, 3, 3, 0, 2, 1> xzy;
         n_swizzle<T, 3, 3, 1, 0, 2> yxz;
         n_swizzle<T, 3, 3, 1, 2, 0> yzx;
         n_swizzle<T, 3, 3, 2, 1, 0> zyx;
         n_swizzle<T, 3, 3, 2, 0, 1> zxy;
      };
   };

   template<typename T>
   struct n_vector<T, 4>
   {
      union
      {
         T Data[4];
      };
   };


   using vec2 = n_vector<float, 2>;
   using ivec2 = n_vector<int32_t, 2>;

   using vec3 = n_vector<float, 3>;
   using ivec3 = n_vector<int32_t, 3>;

   using vec4 = n_vector<float, 4>;
   using ivec4 = n_vector<int32_t, 4>;
}
