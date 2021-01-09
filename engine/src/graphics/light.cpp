#pragma once
#include "light.h"

Light CreateLight(const Vector3 position, const Vector3 color)
{
   return { position, color };
}

PointLight CreatePointLight(const Vector3 position, const Vector3 color, const float linear, const float constant, const float quadractic)
{
   return { position, color, linear, constant, quadractic};
}

Spotlight CreateSpotlight(const Vector3 position, const Vector3 direction, const Vector3 color, const float outerAngle, const float innerAngle)
{
   return { position, direction, color, outerAngle, innerAngle };
}