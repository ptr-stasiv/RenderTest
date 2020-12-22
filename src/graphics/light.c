#pragma once
#include "light.h"

Light CreateLight(const Vector3 position, const Vector3 color)
{
   return (Light){ position, color };
}