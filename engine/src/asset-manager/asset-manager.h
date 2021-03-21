#pragma once
#include <cstdint>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <optional>
#include <mutex>

#include "debug/gassert.h"
#include "utils/sync/spin-lock.h"

namespace assets
{
   enum class AssetType
   {
      None,
      Mesh,
      Image
   };

   #define ASSET_TYPE(type) virtual inline AssetType GetType() const { return type; } \
                            static inline AssetType GetStaticType() { return type; }

   class AssetData
   {
   public:
      struct LoadingInfo
      {
         std::string Name;
         size_t HashedName;
         float LoadTime;
         bool IsValid;
      } Info;

      bool operator == (const AssetData& a)
      {
         return a.Info.HashedName == Info.HashedName;
      }

      ASSET_TYPE(AssetType::None)
   };


   class AssetManager;

   class AssetRef
   {
   public:
      uint16_t Id       = 0;
      AssetManager* AM  = nullptr;

      AssetRef() = default;

      inline AssetRef(const uint16_t id, AssetManager* am)
         : Id(id), AM(am) {}

      template<typename T> 
      inline std::shared_ptr<T>GetData() const
      {
         return AM->GetAssetData<T>(Id);
      }
   };

   class AssetManager
   {
   public:
      std::unordered_map<uint16_t, const char*> AssetInfoLookup;
      std::unordered_map<uint16_t, std::shared_ptr<AssetData>> AssetDataLookup;

      uint16_t AssetInfoCounter = 1;

      utils::sync::SpinLock LoadSL;
   public:
      void Load();

      inline uint16_t RequireAsssetId(const char* filepath)
      {
         AssetInfoLookup.size();
         AssetInfoLookup[AssetInfoCounter] = filepath;
         return AssetInfoCounter++;
      }

      inline AssetRef RequireAsssetRef(const char* filepath)
      {
         AssetInfoLookup[AssetInfoCounter] = filepath;
         return AssetRef(AssetInfoCounter++, this);
      }

      template<typename T>
      inline std::shared_ptr<T> GetAssetData(const uint16_t id) const
      {
         GASSERT(T::GetStaticType() == AssetDataLookup.at(id)->GetType(), "Wrong asset type is requested!");
         GASSERT(AssetDataLookup.at(id)->Info.IsValid, "Invalid asset has been trying being used");

         return std::static_pointer_cast<T>(AssetDataLookup.at(id));
      }
   };
}