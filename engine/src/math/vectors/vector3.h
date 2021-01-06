#pragma once

typedef struct Vector3Structure
{
   float x, y, z;
}Vector3;

Vector3 vec3(const float x, const float y, const float z);
Vector3 svec3(const float s);

Vector3 add_vector3(const Vector3 a, const Vector3 b);

Vector3 subtract_vector3(const Vector3 a, const Vector3 b);

Vector3 muliply_svector3(const Vector3 v, const float s);

Vector3 normalize_vector3(const Vector3 v);

Vector3 cross_vector3(const Vector3 a, const Vector3 b);

float length_vector3(const Vector3 v);

float dot_vector3(const Vector3 a, const Vector3 b);
