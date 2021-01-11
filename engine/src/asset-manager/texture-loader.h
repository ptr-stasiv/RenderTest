#pragma once
#include <cstdint>

#include "asset-manager.h"

struct TextureData
{
   int32_t Width;
   int32_t Height;

   int32_t Channels;

   void* Pixels;
};

TextureData LoadTexture(const char* filepath);