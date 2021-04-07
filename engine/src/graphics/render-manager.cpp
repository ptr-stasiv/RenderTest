#include "render-manager.h"

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
      LightsUBO->InitData((sizeof(PointLightA) + sizeof(SpotlightA)) * (MaxPointLights + MaxSpotlights), nullptr);
   }

   void RenderManager::Update(const Camera& camera)
   {
      LightsUBO->UpdateData(sizeof(PointLightA) * PointLightCounter, PointLightList);
      LightsUBO->UpdateData(sizeof(SpotlightA) * SpotlightCounter, SpotlightList, sizeof(PointLightA) * MaxPointLights);

      for (auto& mesh : MeshList)
      {
         auto& transformmatrix = math::CreateScaleMatrix(mesh.Scale);

         auto& material = mesh.Material;
         material->ShaderProgram->Use();

         material->SetCameraPosition(camera.Position);
         material->SetObjectToWorldMatrix(transformmatrix);
         material->SetWorldToCameraMatrix(camera.GetCameraViewMatrix());
         material->SetCameraToClipMatrix(camera.GetCameraProjection());

         material->SetPointLightsCount(PointLightCounter);
         material->SetSpotlightsCount(SpotlightCounter);

         material->ResolveUniforms();

         PositionsVBO->UpdateData(mesh.Vertices.Positions.size() * sizeof(math::Vector3), &mesh.Vertices.Positions[0]);
         NormalsVBO->UpdateData(mesh.Vertices.Normals.size() * sizeof(math::Vector3), &mesh.Vertices.Normals[0]);
         UVsVBO->UpdateData(mesh.Vertices.UVs.size() * sizeof(math::Vector2), &mesh.Vertices.UVs[0]);
         TangentsVBO->UpdateData(mesh.Vertices.Tangents.size() * sizeof(math::Vector3), &mesh.Vertices.Tangents[0]);

         GD->DrawTriangles(material->ShaderProgram, mesh.Vertices.Positions.size() * 3);
      }
   }
}