#include "forward-render.h"

#include "utils/read-from-file.h"

namespace graphics
{
   constexpr std::string_view VertexShaderLocation = "res/shaders/phong-shading.vs";
   constexpr std::string_view FragmentShaderLocation = "res/shaders/phong-shading.fs";

   ForwardRender::ForwardRender(const std::shared_ptr<graphics::GraphicsDevice>& device)
         : GraphicsDevice(device)
   {  
      MainShader = GraphicsDevice->CreateShaderProgram();
      MainShader->AddShader(ShaderType::Vertex, utils::ReadFromFile(VertexShaderLocation));
      MainShader->AddShader(ShaderType::Fragment, utils::ReadFromFile(FragmentShaderLocation));
      MainShader->Compile();

      
      //VBO's setup 

      PositionsVBO = GraphicsDevice->CreateVBO();
      PositionsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      NormalsVBO = GraphicsDevice->CreateVBO();
      NormalsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      UVsVBO = GraphicsDevice->CreateVBO();
      UVsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector2), nullptr);

      MainShader->AddInputBuffer(PositionsVBO, 3, PositionAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(NormalsVBO, 3, NormalAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(UVsVBO, 2, UvAttribLocation, sizeof(math::Vector2), Type::Float);

   
      //UBO's setup

      LightUBO = GraphicsDevice->CreateUBO();

      LightUBO->InitData((sizeof(PointLightA) + sizeof(SpotlightA)) * (MaxPointLights + MaxSpotlights), nullptr);

      MainShader->AddInputBuffer(LightUBO, "LightBlock", sizeof(PointLightA) * MaxPointLights);


      MaterialUBO = GraphicsDevice->CreateUBO();

      MaterialUBO->InitData(sizeof(MaterialA), nullptr);

      MainShader->AddInputBuffer(MaterialUBO, "MaterialBlock", sizeof(MaterialA));
   }

   void ForwardRender::UpdateLight()
   {
      MainShader->SetInt("PointLightsCount", PointLightCounter);
      MainShader->SetInt("SpotlightsCount", SpotlightCounter);
      
      LightUBO->UpdateData(sizeof(PointLightA) * PointLightCounter, PointLightList);
      LightUBO->UpdateData(sizeof(SpotlightA) * SpotlightCounter, SpotlightList, sizeof(PointLightA) * MaxPointLights); 
   }

   void ForwardRender::Render(const Camera& camera)
   {
      MainShader->Use();

      UpdateLight();

      MainShader->SetFloats("CameraPosition", camera.Position);
      MainShader->SetFloats("Projection", camera.GetCameraProjection());
      MainShader->SetFloats("View", camera.GetCameraViewMatrix());

      for(auto r : RendererList)
      { 
         MaterialA material;
         material.DiffuseColor = r.Material.DiffuseColor;
         material.SpecularColor = r.Material.SpecularColor;
         material.Emissive = r.Material.Emissive;
         material.Glossiness = r.Material.Glossiness;

         MaterialUBO->UpdateData(sizeof(MaterialA), &material);


         MainShader->SetFloats("Model", r.Transformation);

         PositionsVBO->UpdateData(r.Mesh.Positions.size() * sizeof(math::Vector3), &r.Mesh.Positions[0]);
         NormalsVBO->UpdateData(r.Mesh.Normals.size() * sizeof(math::Vector3), &r.Mesh.Normals[0]);
         UVsVBO->UpdateData(r.Mesh.UVs.size() * sizeof(math::Vector2), &r.Mesh.UVs[0]);

         GraphicsDevice->DrawTriangles(MainShader, r.Mesh.Positions.size() * 3);
      }
   }
}
