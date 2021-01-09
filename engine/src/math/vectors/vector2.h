#pragma once

typedef struct Vector2
{
   float x, y;
}Vector2;

Vector2 vec2(const float x, const float y);
Vector2 svec2(const float s);

Vector2 add_Vector2(const Vector2 a, const Vector2 b);

Vector2 subtract_Vector2(const Vector2 a, const Vector2 b);

Vector2 muliply_sVector2(const Vector2 v, const float s);

Vector2 normalize_Vector2(const Vector2 v);

float length_Vector2(const Vector2 v);

float dot_Vector2(const Vector2 a, const Vector2 b);