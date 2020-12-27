#pragma once
#include "../math/vectors/vector3.h"

typedef struct LightStruct
{
   Vector3 Position;
   Vector3 Color;
} Light;

typedef struct PointLightStruct
{
   Vector3 Position;
   Vector3 Color;

   float Linear;
   float Constant;
   float Quadratic;
} PointLight;

Light CreateLight(const Vector3 position, const Vector3 color);
PointLight CreatePointLight(const Vector3 position, const Vector3 color, const float linear, const float constant, const float quadractic);