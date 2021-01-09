#include "material.h"

Material CreateMaterial(const Vector3 color, const Vector3 spec, const float exp, const Vector3 emissive)
{
   return {color, spec, emissive, exp};
}