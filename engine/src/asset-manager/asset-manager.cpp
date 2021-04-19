#include "asset-manager.h"

#include <filesystem>
#include <string_view>

#include "stb/stb_image.h"

#include "jobs/job-system.h"
#include "utils/timer.h"

namespace assets
{
   namespace loaders
   {
      TrigVertices LoadTrigVertices(const std::string_view filepath)
      {
         TrigVertices resData;

         utils::Timer loadTimer(true);

         FILE* file = fopen(&filepath[0], "rb");
         if (!file)
         {
            //TODO when loading fail laod the default model
            LOG_ERROR("Failed to load trig model: %s", filepath);
            return resData;
         }

         std::vector<mm::vec3> positionArray;
         std::vector<mm::vec3> normalArray;
         std::vector<mm::vec2> uvArray;

         std::vector<uint32_t> positionIndicesArray;
         std::vector<uint32_t> normalIndicesArray;
         std::vector<uint32_t> uvIndicesArray;

         while (1)
         {
            char header[128];
            if (fscanf(file, "%s", header) == EOF)
               break;

            if (!strcmp(header, "v"))
            {
               mm::vec3 v;
               fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);

               positionArray.emplace_back(v);
            }
            else if (!strcmp(header, "vn"))
            {
               mm::vec3 v;
               fscanf(file, "%f %f %f", &v.x, &v.y, &v.z);

               normalArray.emplace_back(v);
            }
            else if (!strcmp(header, "vt"))
            {
               mm::vec2 v;
               fscanf(file, "%f %f", &v.x, &v.y);

               uvArray.emplace_back(v);
            }
            else if (!strcmp(header, "f"))
            {
               int p1, p2, p3;
               int n1, n2, n3;
               int t1, t2, t3;

               fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &p1, &t1, &n1, &p2, &t2, &n2, &p3, &t3, &n3);

               positionIndicesArray.emplace_back(p1);
               positionIndicesArray.emplace_back(p2);
               positionIndicesArray.emplace_back(p3);

               normalIndicesArray.emplace_back(n1);
               normalIndicesArray.emplace_back(n2);
               normalIndicesArray.emplace_back(n3);

               uvIndicesArray.emplace_back(t1);
               uvIndicesArray.emplace_back(t2);
               uvIndicesArray.emplace_back(t3);
            }
         }

         std::vector<mm::vec3> resultPositionArray;
         std::vector<mm::vec3> resultNormalArray;
         std::vector<mm::vec2> resultUvArray;

         for (size_t i = 0; i < positionIndicesArray.size(); ++i)
         {
            resultPositionArray.emplace_back(positionArray[positionIndicesArray[i] - 1]);
            resultNormalArray.emplace_back(normalArray[normalIndicesArray[i] - 1]);
            resultUvArray.emplace_back(uvArray[uvIndicesArray[i] - 1]);
         }


         std::vector<mm::vec3> tangentArray;
         std::vector<mm::vec3> bitangentArray;

         for (size_t i = 0; i < resultPositionArray.size(); i += 3)
         {
            mm::vec3 p0 = resultPositionArray[i];
            mm::vec3 p1 = resultPositionArray[i + 1];
            mm::vec3 p2 = resultPositionArray[i + 2];

            mm::vec2 uv0 = resultUvArray[i];
            mm::vec2 uv1 = resultUvArray[i + 1];
            mm::vec2 uv2 = resultUvArray[i + 2];

            mm::vec3 edge1 = p1 - p0;
            mm::vec3 edge2 = p2 - p0;

            mm::vec2 uvEdge1 = uv1 - uv0;
            mm::vec2 uvEdge2 = uv2 - uv0;


            //Find matrix inverse

            float inverseD = 1 / (uvEdge1.x * uvEdge2.y - uvEdge1.y * uvEdge2.x);

            float m00 = uvEdge2.y * inverseD;
            float m01 = -uvEdge1.y * inverseD;
            float m10 = -uvEdge2.x * inverseD;
            float m11 = uvEdge1.x * inverseD;


            mm::vec3 tangent;
            tangent.x = m00 * edge1.x + m01 * edge2.x;
            tangent.y = m00 * edge1.y + m01 * edge2.y;
            tangent.z = m00 * edge1.z + m01 * edge2.z;
            tangent = mm::normalize(tangent);

            mm::vec3 bitangent;
            bitangent.x = m10 * edge1.x + m11 * edge2.x;
            bitangent.y = m10 * edge1.y + m11 * edge2.y;
            bitangent.z = m10 * edge1.z + m11 * edge2.z;
            bitangent = mm::normalize(bitangent);


            tangentArray.push_back(tangent);
            tangentArray.push_back(tangent);
            tangentArray.push_back(tangent);

            bitangentArray.push_back(bitangent);
            bitangentArray.push_back(bitangent);
            bitangentArray.push_back(bitangent);
         }


         resData.FacesCount = positionIndicesArray.size();
         resData.Positions = resultPositionArray;
         resData.Normals = resultNormalArray;
         resData.UVs = resultUvArray;
         resData.Tangents = tangentArray;
         resData.Bitangents = bitangentArray;

         resData.Name = filepath;
         resData.HashedName = std::hash<std::string_view>{}(filepath);
         resData.LoadTime = loadTimer.GetElapsedTime();
         resData.IsValid = true;

         return  resData;
      }

      PixelsData LoadPixels(const std::string_view filepath)
      {
         stbi_set_flip_vertically_on_load(1);

         PixelsData resData;
         resData.LoadTime = 0.0f;
         resData.IsValid  = false;

         resData.Pixels = stbi_load(&filepath[0], &resData.Width, &resData.Height, &resData.Channels, 0);

         if (!resData.Pixels)
         {
            //TODO load default image
            LOG_ERROR("Faild to load image: %s", filepath);
            return resData;
         }

         resData.Name = filepath;
         resData.HashedName = std::hash<std::string_view>{}(filepath);
         resData.LoadTime = 0.0f;
         resData.IsValid = true;

         return resData;
      } 
   }
   std::unordered_map<std::string_view, AssetType> g_AssetTypeLookup =
   {
      {".obj", AssetType::Mesh},
      {".png", AssetType::Image},
      {".jpg", AssetType::Image}
   };

   struct JobLoadParams
   {
      std::string Path;
      Hash HashedPath;
      AssetManager* AM;
   };

   void AssetManager::Load()
   {
      for (auto assetInfo : LoadQueue)
      {
         auto[hashedFilepath, filepath] = assetInfo;

         JobLoadParams* params = new JobLoadParams({ filepath, hashedFilepath , this });

         core::JobSystem::Execute([](uintptr_t args)
            {
               std::unique_ptr<JobLoadParams> params = std::unique_ptr<JobLoadParams>(reinterpret_cast<JobLoadParams*>(args));
               AssetManager* am = params->AM;

               std::filesystem::path path(params->Path);

               auto find = g_AssetTypeLookup.find(path.extension().string());
               AssetType type = find != g_AssetTypeLookup.end() ? find->second : AssetType::None;

               std::shared_ptr<AssetData> assetData;
               switch (type)
               {
               case assets::AssetType::Mesh:
                  {
                     TrigVertices mesh = loaders::LoadTrigVertices(path.string());
                     assetData = std::make_shared<TrigVertices>(mesh);
                  }break;
               case assets::AssetType::Image:
                  {
                     PixelsData image = loaders::LoadPixels(path.string());
                     assetData = std::make_shared<PixelsData>(image);
                  }break;
               default:
                  {
                     PRINT_AND_TERMINATE("This asset cannot be processed by the engine: %s", path.string().c_str());
                  }break;
               }

               std::lock_guard<utils::sync::SpinLock> l(am->LoadSL);
               am->AssetDataLookup[params->HashedPath] = assetData;

            }, reinterpret_cast<uintptr_t>(params));
      }

      LoadQueue.clear();

      core::JobSystem::Wait();
   }
}
