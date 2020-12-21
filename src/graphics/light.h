#pragma once
#include "../math/vectors/vector3.h"

typedef struct LightStruct
{
   Vector3 Position;
   Vector3 Color;
} Light;

Light CreateLight(const Vector3 position, const Vector3 color);