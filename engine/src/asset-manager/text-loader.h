#pragma once
#include "asset-manager.h"

namespace assets
{
   class TextAssetData : public AssetData
   {
   public:
      std::string TextStr;

      inline TextAssetData(const std::string& str)
         : TextStr(str) {}

      ASSET_TYPE(AssetType::None)
   };

   TextAssetData LoadText(const std::string_view filepath);
}