#include "render-manager.h"

#include <algorithm>

#include "entry-point/global_systems.h"

namespace graphics
{
   struct BoundingSphere
   {
      mm::vec3 Center;
      mm::vec3 Point;
   };

   RenderManager::RenderManager(const std::shared_ptr<GraphicsDevice>& gd)
      : GD(gd)
   {
      RenderCfg cfg;

      //Enable face culling
      GD->SetCullingFace(graphics::Face::Back);

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

      RenderCfgUBO = gd->CreateUBO();
      RenderCfgUBO->InitData(sizeof(RenderCfg), nullptr);
      RenderCfgUBO->UpdateData(sizeof(RenderCfg), &cfg, 0);
      
      
      graphics::TextureParams shadowTextureParams;
      shadowTextureParams.MagFilter = TextureFilter::Nearest;
      shadowTextureParams.MinFilter = TextureFilter::Nearest;
      shadowTextureParams.WrapS = TextureWrap::ClampToBorder;
      shadowTextureParams.WrapT = TextureWrap::ClampToBorder;

      GeneralShadowMap = GD->CreateTexture2D();
      GeneralShadowMap->SetBorderColor(mm::vec4(1.0f));
      GeneralShadowMap->InitData(cfg.ShadowWidth, cfg.ShadowHeight, InternalFormat::Depth24, Format::Depth, Type::Uint, shadowTextureParams);

      GeneralCubeShadowMap = GD->CreateCubemap();
      GeneralCubeShadowMap->SetBorderColor(mm::vec4(1.0f));
      GeneralCubeShadowMap->InitData(cfg.ShadowWidth, cfg.ShadowHeight, InternalFormat::Depth24, Format::Depth, Type::Uint, shadowTextureParams);

      GeneralShadowFBO = GD->CreateFBO();
   }

   void RenderManager::ShadowPass(const Camera& camera)
   {
      GeneralShadowFBO->Bind();

      GD->SetViewport({ 0, 0 }, { GeneralShadowMap->GetSizeX(), 
                                  GeneralShadowMap->GetSizeY() });

      GD->EnableFeature(graphics::Feature::Depth);

      //Cause artifacts
      //GD->SetCullingFace(graphics::Face::Front);


      GeneralShadowFBO->AttachTexture2D(graphics::Attachment::Depth, GeneralShadowMap);

      for (size_t i = 0; i < SpotlightCounter; ++i)
      {
         GD->Clear();

         auto& sl = SpotlightList[i];

         float textureAR = GeneralShadowMap->GetSizeX()
                           / GeneralShadowMap->GetSizeY();

         Camera lightCamera(sl.Position, sl.Direction, textureAR, sl.OuterAngle, 0.0f);

         sl.Camera = mm::transpose(lightCamera.GetCameraProjection() * lightCamera.GetCameraViewMatrix());

         sl.FrustrumWidth = lightCamera.Fov * lightCamera.Aspect * 2;

         GeometryPass(lightCamera);

         ShadowMaps[i] = GeneralShadowMap;
      }

      
      for (size_t i = 0; i < PointLightCounter; ++i)
      {
         static std::unordered_map<CubeFace, mm::vec3> cubeFaceDirections =
         {
            { CubeFace::Top, mm::vec3(0.0f, 1.0f, 0.0f) },
            { CubeFace::Bottom, mm::vec3(0.0f, -1.0f, 0.0f) },
            { CubeFace::Front, mm::vec3(0.0f, 0.0f, 1.0f) },
            { CubeFace::Backward, mm::vec3(0.0f, 0.0f, -1.0f) },
            { CubeFace::Left, mm::vec3(-1.0f, 0.0f, 0.0f) },
            { CubeFace::Right, mm::vec3(1.0f, 0.0f, 0.0f) }
         };

         for (uint8_t f = 0; f < 6; ++f)
         {
            GeneralShadowFBO->AttachTexture2D(graphics::Attachment::Depth, static_cast<CubeFace>(f), GeneralCubeShadowMap);

            GD->Clear();

            
            auto& pl = PointLightList[i];

            float textureAR = GeneralShadowMap->GetSizeX()
                              / GeneralShadowMap->GetSizeY();

            Camera lightCamera(pl.Position, cubeFaceDirections.at(static_cast<CubeFace>(f)), textureAR, 1.0f, 0.0f);

            pl.Cameras[f] = mm::transpose(lightCamera.GetCameraProjection() * lightCamera.GetCameraViewMatrix());

            pl.FrustrumWidth = lightCamera.Fov * lightCamera.Aspect * 2;

            GeometryPass(lightCamera);

            CubeShadowMaps[i] = GeneralCubeShadowMap;
         }
      }


      GD->SetCullingFace(graphics::Face::Back);

      GD->SetViewport({ 0, 0 }, { g_Window->GetCanvas()->GetWidth(), 
                                  g_Window->GetCanvas()->GetHeight() });

      GeneralShadowFBO->Unbind();
   }

   void RenderManager::LightPass(const Camera& camera)
   {
      LightsUBO->UpdateData(sizeof(PointLightAligned16) * PointLightCounter, PointLightList);
      LightsUBO->UpdateData(sizeof(SpotlightAligned16) * SpotlightCounter, SpotlightList, sizeof(PointLightAligned16) * MaxPointLights);

      std::vector<BoundingSphere> BoundingSphereList;

      //Debug the tiled rendering
      for (size_t i = 0; i < PointLightCounter; ++i)
      {
         auto& pl = PointLightList[i];

         float disc = 4 * pl.Offset * pl.Offset - 4 * (pl.Offset * pl.Offset - 10 * pl.Stretch); //10 is the reverse fraction of 1/10 because here we solve fraction equation
                                                                                                 //This number means what maximum attenuation we will account

         float x = (-2 * pl.Offset + sqrt(disc)) / 2;

         g_DebugManager->AddAASphere({ 1.0f }, 24, PointLightList[i].Position, x);

         BoundingSphere sphere;
         sphere.Center = pl.Position;
         sphere.Point = pl.Position + mm::vec4(x, 0.0f, 0.0f, 0.0f);

         BoundingSphereList.push_back(sphere);
      }

      GeometryPass(camera);

      //Scene update render data every frame
      //So this values if fully useable till the light pass
      PointLightCounter = 0;
      SpotlightCounter = 0;
   }

   void RenderManager::GeometryPass(const Camera& camera)
   {
      std::sort(CurrentRenderQueue.begin(), CurrentRenderQueue.end(),
                [](auto& left, auto& right)
                {
                   return left.first > right.first;
                });

      for (auto& renderer : CurrentRenderQueue)
      {
         RenderKey lastKey = { INT64_MAX };

         auto& mesh = renderer.second;

         //TODO implement rotation
         mm::mat4 worldTransform;
         worldTransform = mm::translate(worldTransform, mesh.Translate);
         worldTransform = mm::scale(worldTransform, mesh.Scale);

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
   }

   void RenderManager::Update(const Camera& camera)
   {
      ShadowPass(camera);

      LightPass(camera);


      CurrentRenderQueue.clear();

      PointLightCounter = 0;
      SpotlightCounter = 0;
   }
}
