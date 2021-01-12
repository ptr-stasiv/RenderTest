#include "asset-manager.h"

#include <filesystem>
#include <string_view>

#include "obj-loader.h"
#include "image-loader.h"
#include "text-loader.h"

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
               TextAssetData text = LoadText(path.string());
               assetData = std::make_shared<TextAssetData>(text);
            }break;
         case assets::AssetType::Mesh:
            {
               MeshAssetData mesh = LoadMesh(path.string());
               assetData = std::make_shared<MeshAssetData>(mesh);
            }break;
         case assets::AssetType::Image:
            {
               ImageAssetData image = LoadImage(path.string());
               assetData = std::make_shared<ImageAssetData>(image);
            }break;
         }

         if (!assetData->Info.IsValid)
            LOG_ERROR("Error loading asset in path: %s", path.string().c_str());

         AssetDataLookup[assetInfo.first] = assetData;
      }
   }
}