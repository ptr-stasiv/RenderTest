#pragma once
#include <cstdint>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <optional>
#include <mutex>

#include "math/vectors/vector.h"

#include "debug/globals.h"
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
      std::string Name = "";//Later this with HasedName  should be replaced with one type that implement hash string

      size_t HashedName = -1;

      float LoadTime = 0.0f;

      bool IsValid = false;

      ASSET_TYPE(AssetType::None)
   };

   class TrigVertices : public AssetData
   {
   public:
      std::vector<math::Vector3> Positions;

      std::vector<math::Vector3> Normals;

      std::vector<math::Vector2> UVs;

      std::vector<math::Vector3> Tangents;

      std::vector<math::Vector3> Bitangents;

      ASSET_TYPE(AssetType::Mesh);
   };

   class PixelsData : public AssetData
   {
   public:
      int32_t Width;

      int32_t Height;

      int32_t Channels;

      void* Pixels;

      ASSET_TYPE(AssetType::Image)
   }; 

   class AssetManager
   {
   public:
      std::vector<std::pair<size_t, std::string>> AssetInfoLookup;
      std::unordered_map<size_t, std::shared_ptr<AssetData>> AssetDataLookup;

      utils::sync::SpinLock LoadSL;
   public:
      void Load();

      inline size_t LoadAsset(const std::string_view& filepath)
      {
         AssetInfoLookup.size();
         AssetInfoLookup[AssetInfoCounter] = filepath;
         return AssetInfoCounter++;
      }

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
         ASSERT(T::GetStaticType() == AssetDataLookup.at(id)->GetType(), "Wrong asset type is requested!");
         ASSERT(AssetDataLookup.at(id)->Info.IsValid, "Invalid asset has been trying being used");

         return std::static_pointer_cast<T>(AssetDataLookup.at(id));
      }
   };
}
