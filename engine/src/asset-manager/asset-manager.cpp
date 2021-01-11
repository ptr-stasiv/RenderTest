#include "asset-manager.h"

#include <filesystem>
#include <string_view>

#include "obj-loader.h"
#include "texture-loader.h"

namespace assets
{
   static std::unordered_map<std::string_view, AssetType> g_AssetTypeLookup =
   {
      {".obj", AssetType::Mesh},
      {".png", AssetType::Image},
      {".jpg", AssetType::Image}
   };

   void AssetManager::Load()
   {
      for (auto assetInfo : AssetInfoLookup)
      {
         std::filesystem::path path(assetInfo.second);

         auto find = g_AssetTypeLookup.find(path.extension().string());
         AssetType type = find != g_AssetTypeLookup.end() ? find->second : AssetType::None;

         std::shared_ptr<AssetData> assetData;
         switch (type)
         {
         case assets::AssetType::None:
            {
               assetData = std::make_shared<AssetData>();
            }break;
         case assets::AssetType::Mesh:
            {
               MeshAssetData mesh = LoadMesh(path.string().c_str());
               assetData = std::make_shared<MeshAssetData>(mesh);
            }break;
         case assets::AssetType::Image:
            {

            }break;
         }

         AssetDataLookup[assetInfo.first] = assetData;
      }
   }
}