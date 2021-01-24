#include "asset-manager.h"

#include <filesystem>
#include <string_view>

#include "obj-loader.h"
#include "image-loader.h"
#include "text-loader.h"

#include "jobs/job-system.h"

namespace assets
{
   static std::unordered_map<std::string_view, AssetType> g_AssetTypeLookup =
   {
      {".obj", AssetType::Mesh},
      {".png", AssetType::Image},
      {".jpg", AssetType::Image}
   };

   struct JobLoadParams
   {
      std::string Path;
      uint16_t Hash;
      AssetManager* AM;
   };

   void AssetManager::Load()
   {
      for (auto assetInfo : AssetInfoLookup)
      {
         JobLoadParams* params = new JobLoadParams({ assetInfo.second, assetInfo.first, this });

         core::JobSystem::Execute([](uintptr_t args)
            {
               std::unique_ptr<JobLoadParams> params = std::unique_ptr<JobLoadParams>(reinterpret_cast<JobLoadParams*>(args));
               AssetManager* am = params->AM;

               am->LoadMutex.lock();

               std::filesystem::path path(params->Path);

               auto find = g_AssetTypeLookup.find(path.extension().string());
               AssetType type = find != g_AssetTypeLookup.end() ? find->second : AssetType::None;

               am->LoadMutex.unlock();

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

               am->LoadMutex.lock();

               am->AssetDataLookup[params->Hash] = assetData;

               am->LoadMutex.unlock();

            }, reinterpret_cast<uintptr_t>(params));
      }

      core::JobSystem::Wait();
   }
}