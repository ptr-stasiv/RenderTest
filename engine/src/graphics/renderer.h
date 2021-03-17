#pragma once
#include "mesh.h"
#include "light/material.h"
#include "math/matrices/matrix4x4.h"

namespace graphics
{
   struct Renderer
   {
      graphics::Mesh Mesh;
      graphics::Material Material;
      math::Matrix4 Transformation;
   };
}