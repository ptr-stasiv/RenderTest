#pragma once

typedef struct Vector4Structure
{
   float x, y, z, w;
}Vector4;


Vector4 add_vector4(const Vector4 a, const Vector4 b);

Vector4 subtract_vector4(const Vector4 a, const Vector4 b);

Vector4 muliply_svector4(const Vector4 v, const float s);

float dot_vector4(const Vector4 a, const Vector4 b);