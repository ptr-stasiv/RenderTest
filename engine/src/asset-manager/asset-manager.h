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
   typedef size_t Hash;

   static Hash GetHash(const std::string_view& str)
   {
      return std::hash<std::string_view>{}(str);
   }


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

      Hash HashedName = -1;

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

      Hash FacesCount;

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
   private:
      std::vector<std::pair<Hash, std::string>> LoadQueue;
      std::unordered_map<Hash, std::shared_ptr<AssetData>> AssetDataLookup;

      utils::sync::SpinLock LoadSL;

      template<typename T>
      inline std::shared_ptr<T> GetData(const Hash hash) const
      {
         auto asset = AssetDataLookup.find(hash);

         if (asset == AssetDataLookup.end()
            || T::GetStaticType() != asset->second->GetType()
            || !asset->second->IsValid)
         {
            PRINT_AND_TERMINATE("Invalid asset was trying to being used: %s", asset->second->Name.c_str());
         }

         return std::static_pointer_cast<T>(AssetDataLookup.at(hash));
      }
   public:
      void Load();

      inline void ToLoad(const std::string_view& filepath)
      {
         LoadQueue.emplace_back(GetHash(filepath), filepath);
      }

      template<typename T>
      inline std::shared_ptr<T> GetData(const std::string_view& filepath) const
      {
         const Hash hash = GetHash(filepath);
         return GetData<T>(hash);
      }
   };
}
