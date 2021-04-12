#include "render-manager.h"

#include <algorithm>

namespace graphics
{
   RenderManager::RenderManager(const std::shared_ptr<GraphicsDevice>& gd)
      : GD(gd)
   {
      //Setup VBO's

      PositionsVBO = GD->CreateVBO();
      PositionsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      NormalsVBO = GD->CreateVBO();
      NormalsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      UVsVBO = GD->CreateVBO();
      UVsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector2), nullptr);

      TangentsVBO = GD->CreateVBO();
      TangentsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      
      //UBO's setup
      LightsUBO = gd->CreateUBO();
      LightsUBO->InitData((sizeof(PointLightAligned16) + sizeof(SpotlightAligned16)) * (MaxPointLights + MaxSpotlights), nullptr);
   }

   void RenderManager::Update(const Camera& camera)
   {
      LightsUBO->UpdateData(sizeof(PointLightAligned16) * PointLightCounter, PointLightList);
      LightsUBO->UpdateData(sizeof(SpotlightAligned16) * SpotlightCounter, SpotlightList, sizeof(PointLightAligned16) * MaxPointLights);


      std::sort(CurrentRenderQueue.begin(), CurrentRenderQueue.end(), [](auto& left, auto& right)
         {
            return left.first > right.first;
         });

      for (auto& renderer : CurrentRenderQueue)
      {
         RenderKey lastKey = { INT64_MAX };

         auto& mesh = renderer.second;

         auto& transformmatrix = math::CreateScaleMatrix(mesh.Scale);

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

         material->SetObjectToWorldMatrix(transformmatrix);
         material->ResolveUniforms();

         PositionsVBO->UpdateData(mesh.Vertices.Positions.size() * sizeof(math::Vector3), &mesh.Vertices.Positions[0]);
         NormalsVBO->UpdateData(mesh.Vertices.Normals.size() * sizeof(math::Vector3), &mesh.Vertices.Normals[0]);
         UVsVBO->UpdateData(mesh.Vertices.UVs.size() * sizeof(math::Vector2), &mesh.Vertices.UVs[0]);
         TangentsVBO->UpdateData(mesh.Vertices.Tangents.size() * sizeof(math::Vector3), &mesh.Vertices.Tangents[0]);

         GD->DrawTriangles(material->ShaderProgram, mesh.Vertices.Positions.size() * 3);
      }

      CurrentRenderQueue.clear();

      PointLightCounter = 0;
      SpotlightCounter = 0;
   }
}
