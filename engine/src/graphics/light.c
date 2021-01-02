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