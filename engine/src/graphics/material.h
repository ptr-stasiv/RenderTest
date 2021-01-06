#pragma once
#include "math/vectors/vector3.h"

typedef struct Material
{
   Vector3 Color;
   Vector3 Specular;
   Vector3 Emissive;
   float   ShineExponent;
} Material;

Material CreateMaterial(const Vector3 color, const Vector3 spec, const float exp, const Vector3 emissive);