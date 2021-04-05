#pragma once
#include <cstdint>

#include "asset-manager.h"

namespace assets
{
   class ImageAssetData : public AssetData
   {
   public:
      int32_t Width;
      int32_t Height;

      int32_t Channels;

      void* Pixels;

      ASSET_TYPE(AssetType::Image)
   };

   ImageAssetData LoadImage(const std::string_view filepath);
}
