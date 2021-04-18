#pragma once
#include <cstdint>
#include <cstring>
#include <algorithm>

namespace mm
{
   template<typename T, size_t Components>
   struct n_vector;

   template<typename T, size_t Components, size_t ResultComponents, size_t ...ComponentsPermutation>
   struct n_swizzle
   {
      T AliasedData[Components];

      inline operator n_vector<T, ResultComponents> ()
      {
         T resData[ResultComponents] = { AliasedData[ComponentsPermutation]... };

         n_vector<T, ResultComponents> resVector;

         memcpy_s(resVector.Data, sizeof(T) * ResultComponents,
                  resData, sizeof(T) * ResultComponents);

         return resVector;
      }
   };

#define VECTOR_CONSTRUCTORS(components) \
      inline n_vector() \
         : n_vector(0.0f) {} \
      inline n_vector(const T& scalar) \
      { \
         memset(Data, scalar, sizeof(T) * components); \
      } \
      template<typename T, size_t Components2> \
      inline n_vector(const n_vector<T, Components2>& v2) \
         : n_vector() \
      { \
         memcpy_s(Data, sizeof(T) * components, v2.Data, sizeof(T) * min(components, Components2)); \
      } \
      template<size_t Components2> \
      inline n_vector<T, components>& operator = (const n_vector<T, Components2>& v2) \
      { \
         memcpy_s(Data, sizeof(T) * components, v2.Data, sizeof(T) * min(components, Components2)); \
         return *this; \
      }

   template<typename T>
   struct n_vector<T, 2>
   {
      union
      {
         T Data[2];

         struct { T x, y; };

         n_swizzle<T, 2, 2, 0, 1> xy;
         n_swizzle<T, 2, 2, 1, 0> yx;
      };

      VECTOR_CONSTRUCTORS(2)
   };

   template<typename T>
   struct n_vector<T, 3>
   {
      union
      {
         T Data[3];

         struct { T x, y, z; };

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

      VECTOR_CONSTRUCTORS(3)
   };

   template<typename T>
   struct n_vector<T, 4>
   {
      union
      {
         T Data[4];

         struct { T x, y, z, w; };

         n_swizzle<T, 4, 2, 0, 1> xy;
         n_swizzle<T, 4, 2, 1, 0> yx;
         n_swizzle<T, 4, 2, 0, 2> xz;
         n_swizzle<T, 4, 2, 2, 0> zx;
         n_swizzle<T, 4, 2, 0, 3> xw;
         n_swizzle<T, 4, 2, 3, 0> wx;
         n_swizzle<T, 4, 2, 1, 2> yz;
         n_swizzle<T, 4, 2, 2, 1> zy;
         n_swizzle<T, 4, 2, 1, 3> yw;
         n_swizzle<T, 4, 2, 3, 1> wy;
         n_swizzle<T, 4, 2, 2, 3> zw;
         n_swizzle<T, 4, 2, 3, 2> wz;

         n_swizzle<T, 4, 3, 0, 1, 2> xyz;
         n_swizzle<T, 4, 3, 0, 2, 1> xzy;
         n_swizzle<T, 4, 3, 0, 1, 3> xyw;
         n_swizzle<T, 4, 3, 0, 3, 1> xwy;
         n_swizzle<T, 4, 3, 0, 2, 3> xzw;
         n_swizzle<T, 4, 3, 0, 3, 2> xwz;
         n_swizzle<T, 4, 3, 1, 0, 2> yxz;
         n_swizzle<T, 4, 3, 1, 2, 0> yzx;
         n_swizzle<T, 4, 3, 1, 0, 3> yxw;
         n_swizzle<T, 4, 3, 1, 3, 0> ywx;
         n_swizzle<T, 4, 3, 1, 2, 3> yzw;
         n_swizzle<T, 4, 3, 1, 3, 2> ywz;
         n_swizzle<T, 4, 3, 2, 0, 1> zxy;
         n_swizzle<T, 4, 3, 2, 1, 0> zyx;
         n_swizzle<T, 4, 3, 2, 0, 3> zxw;
         n_swizzle<T, 4, 3, 2, 3, 0> zwx;
         n_swizzle<T, 4, 3, 2, 1, 3> zyw;
         n_swizzle<T, 4, 3, 2, 3, 1> zwy;
         n_swizzle<T, 4, 3, 3, 0, 1> wxy;
         n_swizzle<T, 4, 3, 3, 1, 0> wyx;
         n_swizzle<T, 4, 3, 3, 0, 2> wxz;
         n_swizzle<T, 4, 3, 3, 2, 0> wzx;
         n_swizzle<T, 4, 3, 3, 1, 2> wyz;
         n_swizzle<T, 4, 3, 3, 2, 1> wzy;

         n_swizzle<T, 4, 4, 0, 1, 2, 3> xyzw;
         n_swizzle<T, 4, 4, 0, 1, 3, 2> xywz;
         n_swizzle<T, 4, 4, 0, 2, 1, 3> xzyw;
         n_swizzle<T, 4, 4, 0, 2, 3, 1> xzwy;
         n_swizzle<T, 4, 4, 0, 3, 1, 2> xwyz;
         n_swizzle<T, 4, 4, 0, 3, 2, 1> xwzy;
         n_swizzle<T, 4, 4, 1, 0, 2, 3> yxzw;
         n_swizzle<T, 4, 4, 1, 0, 3, 2> yxwz;
         n_swizzle<T, 4, 4, 1, 2, 0, 3> yzxw;
         n_swizzle<T, 4, 4, 1, 2, 3, 0> yzwx;
         n_swizzle<T, 4, 4, 1, 3, 0, 2> ywxz;
         n_swizzle<T, 4, 4, 1, 3, 2, 0> ywzx;
         n_swizzle<T, 4, 4, 2, 0, 1, 3> zxyw;
         n_swizzle<T, 4, 4, 2, 0, 3, 1> zxwy;
         n_swizzle<T, 4, 4, 2, 1, 0, 3> zyxw;
         n_swizzle<T, 4, 4, 2, 1, 3, 0> zywx;
         n_swizzle<T, 4, 4, 2, 3, 0, 1> zwxy;
         n_swizzle<T, 4, 4, 2, 3, 1, 0> zwyx;
         n_swizzle<T, 4, 4, 3, 0, 1, 2> wxyz;
         n_swizzle<T, 4, 4, 3, 0, 2, 1> wxzy;
         n_swizzle<T, 4, 4, 3, 1, 0, 2> wyxz;
         n_swizzle<T, 4, 4, 3, 1, 2, 0> wyzx;
         n_swizzle<T, 4, 4, 3, 2, 0, 1> wzxy;
         n_swizzle<T, 4, 4, 3, 2, 1, 0> wzyx;
      };

      VECTOR_CONSTRUCTORS(4)
   };

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator + (const n_vector<T, Components>& v1, const n_vector<T, Components>& v2)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v1.Data[i] + v2.Data[i];

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator - (const n_vector<T, Components>& v1, const n_vector<T, Components>& v2)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v1.Data[i] - v2.Data[i];

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator * (const n_vector<T, Components>& v1, const n_vector<T, Components>& v2)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v1.Data[i] * v2.Data[i];

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator / (const n_vector<T, Components>& v1, const n_vector<T, Components>& v2)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v1.Data[i] / v2.Data[i];

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator + (const n_vector<T, Components>& v, const float scalar)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v.Data[i] + scalar;

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator - (const n_vector<T, Components>& v, const float scalar)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v.Data[i] - scalar;

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator * (const n_vector<T, Components>& v, const float scalar)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v.Data[i] * scalar;

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator / (const n_vector<T, Components>& v, const float scalar)
   {
      n_vector<T, Components> result;

      for(size_t i = 0; i < Components; ++i)
         result.Data[i] = v.Data[i] / scalar;

      return result;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator += (n_vector<T, Components>& destV, const n_vector<T, Components>& srcV)
   {
      return destV = destV + srcV;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator -= (n_vector<T, Components>& destV, const n_vector<T, Components>& srcV)
   {
      return destV = destV - srcV;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator *= (n_vector<T, Components>& destV, const n_vector<T, Components>& srcV)
   {
      return destV = destV * srcV;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator /= (n_vector<T, Components>& destV, const n_vector<T, Components>& srcV)
   {
      return destV = destV / srcV;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator += (n_vector<T, Components>& destV, const float srcS)
   {
      return destV = destV + srcS;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator -= (n_vector<T, Components>& destV, const float srcS)
   {
      return destV = destV - srcS;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator *= (n_vector<T, Components>& destV, const float srcS)
   {
      return destV = destV * srcS;
   }

   template<typename T, size_t Components>
   inline n_vector<T, Components> operator /= (n_vector<T, Components>& destV, const float srcS)
   {
      return destV = destV / srcS;
   }

   template<typename T, size_t Components>
   inline float dot(const n_vector<T, Components>& v1, const n_vector<T, Components>& v2)
   {
      float res = 0.0f;
      for(size_t i = 0; i < Components; ++i)
         res += v1.Data[i] * v2.Data[i];

      return res;
   }

   template<typename T, size_t Components>
   inline float length(const n_vector<T, Components>& v)
   {
      return sqrt(dot(v, v));
   }
 
   template<typename T, size_t Components>
   inline n_vector<T, Components> normalize(const n_vector<T, Components>& v)
   {
      return v / length(v);
   }

   template<typename T>
   inline n_vector<T, 3> cross(const n_vector<T, 3>& v1, const n_vector<T, 3>& v2)
   {
      n_vector<T, 3> res;
      res.x = v1.y * v2.z - v1.z * v2.y;
      res.y = v1.z * v2.x - v1.x * v2.z;
      res.z = v1.x * v2.y - v1.y * v2.x;

      return res;
   }

   using vec2 = n_vector<float, 2>;
   using ivec2 = n_vector<int32_t, 2>;

   using vec3 = n_vector<float, 3>;
   using ivec3 = n_vector<int32_t, 3>;

   using vec4 = n_vector<float, 4>;
   using ivec4 = n_vector<int32_t, 4>;
}
