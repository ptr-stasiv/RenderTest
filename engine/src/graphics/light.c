#pragma once
#include "light.h"

Light CreateLight(const Vector3 position, const Vector3 color)
{
   return (Light){ position, color };
}

PointLight CreatePointLight(const Vector3 position, const Vector3 color, const float linear, const float constant, const float quadractic)
{
   return (PointLight){ position, color, linear, constant, quadractic};
}

Spotlight CreateSpotlight(const Vector3 position, const Vector3 direction, const Vector3 color, const float outerAngle, const float innerAngle)
{
   return (Spotlight){ position, direction, color, outerAngle, innerAngle };
}