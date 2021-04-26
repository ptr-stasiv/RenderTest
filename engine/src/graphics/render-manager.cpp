#include "render-manager.h"

#include <algorithm>

#include "entry-point/global_systems.h"

namespace graphics
{
   RenderManager::RenderManager(const std::shared_ptr<GraphicsDevice>& gd)
      : GD(gd)
   {
      //Setup VBO's

      PositionsVBO = GD->CreateVBO();
      PositionsVBO->InitData(MaxVerticesPerDraw * sizeof(mm::vec3), nullptr);

      NormalsVBO = GD->CreateVBO();
      NormalsVBO->InitData(MaxVerticesPerDraw * sizeof(mm::vec3), nullptr);

      UVsVBO = GD->CreateVBO();
      UVsVBO->InitData(MaxVerticesPerDraw * sizeof(mm::vec2), nullptr);

      TangentsVBO = GD->CreateVBO();
      TangentsVBO->InitData(MaxVerticesPerDraw * sizeof(mm::vec3), nullptr);

      
      //UBO's setup
      LightsUBO = gd->CreateUBO();
      LightsUBO->InitData((sizeof(PointLightAligned16) + sizeof(SpotlightAligned16)) * (MaxPointLights + MaxSpotlights), nullptr);
   }

   void RenderManager::Update(const Camera& camera)
   {
      LightsUBO->UpdateData(sizeof(PointLightAligned16) * PointLightCounter, PointLightList);
      LightsUBO->UpdateData(sizeof(SpotlightAligned16) * SpotlightCounter, SpotlightList, sizeof(PointLightAligned16) * MaxPointLights);

      struct alignas(16) BoundingSphere
      {
         mm::vec4 Center;
         float Radius;
      };

      std::vector<BoundingSphere> BoundingSphereList;

      for (size_t i = 0; i < PointLightCounter; ++i)
      {
         auto& pl = PointLightList[i];

         float disc = 4 * pl.Offset * pl.Offset - 4 * (pl.Offset * pl.Offset - 10 * pl.Stretch); //10 is the reverse fraction of 1/10 because here we solve fraction equation
                                                                                                 //This number means what maximum attenuation we will account

         float x = (-2 * pl.Offset + sqrt(disc)) / 2;

         g_DebugManager->AddAASphere({ 1.0f }, 24, PointLightList[i].Position, x);

         BoundingSphereList.push_back({ pl.Position, x });
      }


      std::sort(CurrentRenderQueue.begin(), CurrentRenderQueue.end(), [](auto& left, auto& right)
         {
            return left.first > right.first;
         });

      for (auto& renderer : CurrentRenderQueue)
      {
         RenderKey lastKey = { INT64_MAX };

         auto& mesh = renderer.second;

         mm::mat4 worldTransform;
         worldTransform = mm::translate(worldTransform, mesh.Translate);
         worldTransform = mm::scale(worldTransform, mesh.Scale);
         //mm::rotate(mesh.Rotation.w, mm::vec3(mesh.Rotation.x, mesh.Rotation.y, mesh.Rotation.x));

         auto& material = mesh.Material;

         if (renderer.first.MaterialId != lastKey.MaterialId)
         {
            material->ShaderProgram->Use();
           
            material->SetCameraPosition(camera.Position);
            material->SetWorldToCameraMatrix(camera.GetCameraViewMatrix());
            material->SetCameraToClipMatrix(camera.GetCameraProjection());

            material->SetPointLightsCount(PointLightCounter);
            material->SetSpotlightsCount(SpotlightCounter);

            material->ResolveUniforms();

         }

         material->SetObjectToWorldMatrix(worldTransform);
         material->ResolveUniforms();

         PositionsVBO->UpdateData(mesh.Vertices.Positions.size() * sizeof(mm::vec3), &mesh.Vertices.Positions[0]);
         NormalsVBO->UpdateData(mesh.Vertices.Normals.size() * sizeof(mm::vec3), &mesh.Vertices.Normals[0]);
         UVsVBO->UpdateData(mesh.Vertices.UVs.size() * sizeof(mm::vec2), &mesh.Vertices.UVs[0]);
         TangentsVBO->UpdateData(mesh.Vertices.Tangents.size() * sizeof(mm::vec3), &mesh.Vertices.Tangents[0]);

         GD->DrawTriangles(material->ShaderProgram, mesh.Vertices.Positions.size());
      }

      CurrentRenderQueue.clear();

      PointLightCounter = 0;
      SpotlightCounter = 0;
   }
}
