#pragma once
#include "mesh-render.h"
#include "entry-point/global_systems.h"

#include <sstream>

namespace graphics
{
   BaseMaterial::BaseMaterial(const std::string_view& vertexShaderPath, const std::string_view& fragmentShaderPath,
                              const std::string_view& geomShaderPath)
      : InstanceId(GenerateMaterialId())
   {
      ShaderProgram = g_GraphicsDevice->CreateShaderProgram();

      std::string_view vertexShaderSrc;
      std::string_view fragmentShaderSrc;

      if ((vertexShaderSrc = utils::ReadFromFile(vertexShaderPath)) == "")
         PRINT_AND_TERMINATE("Couldn't read vertex shader file: %s", &vertexShaderPath[0]);  
      if ((fragmentShaderSrc = utils::ReadFromFile(fragmentShaderPath)) == "")
         PRINT_AND_TERMINATE("Couldn't read fragment shader file: %s", &fragmentShaderPath[0]);

      ShaderProgram->AddShader(ShaderType::Vertex, vertexShaderSrc);
      ShaderProgram->AddShader(ShaderType::Fragment, fragmentShaderSrc);

      if (!geomShaderPath.empty())
      {
         std::string_view geomShaderSrc;
         if ((geomShaderSrc = utils::ReadFromFile(geomShaderPath)) == "")
            PRINT_AND_TERMINATE("Couldn't read geometry shader file: %s", &geomShaderPath[0]);

         ShaderProgram->AddShader(ShaderType::Geometry, geomShaderSrc);
      }

      ShaderProgram->Compile();
   }

   PhongMaterial::PhongMaterial()
      : BaseMaterial("res/shaders/phong-shading.vs",
                     "res/shaders/phong-shading.fs")
   {
      ShaderProgram->AddInputBuffer(g_RenderManager->PositionsVBO, 3, 0, sizeof(mm::vec3), Type::Float);
      ShaderProgram->AddInputBuffer(g_RenderManager->NormalsVBO, 3, 1, sizeof(mm::vec3), Type::Float);
      ShaderProgram->AddInputBuffer(g_RenderManager->UVsVBO, 2, 2, sizeof(mm::vec2), Type::Float);
      ShaderProgram->AddInputBuffer(g_RenderManager->TangentsVBO, 3, 3, sizeof(mm::vec3), Type::Float);

      ShaderProgram->AddInputBuffer(g_RenderManager->LightsUBO, "LightBlock", (sizeof(PointLightAligned16) + sizeof(SpotlightAligned16)) 
                                                                               * (MaxPointLights + MaxSpotlights));
      ShaderProgram->AddInputBuffer(g_RenderManager->RenderCfgUBO, "RenderCfgBlock", sizeof(RenderCfg), 0);
   }
   
   void PhongMaterial::ResolveUniforms()
   {
      ShaderProgram->SetFloats("Diffuse", Diffuse);

      ShaderProgram->SetFloats("Specular", Specular);

      ShaderProgram->SetFloats("Emissive", Emissive);

      ShaderProgram->SetFloat("Glossiness", Glossiness);


      ShaderProgram->SetTexture2D("DiffuseTexture", DiffuseTexture);
      ShaderProgram->SetTexture2D("NormalTexture", NormalTexture);

      for (size_t i = 0; i < g_RenderManager->GetSpotlightsCount(); ++i)
      {
         //Temoral solution, needed because in the shadow pass this function is called as well as in the main pass
         if (!g_RenderManager->ShadowMaps[i])
            continue;

         std::stringstream name;
         name << "ShadowMaps[" << i << ']';

         ShaderProgram->SetTexture2D(name.str(), g_RenderManager->ShadowMaps[i]);
      }

      for (size_t i = 0; i < g_RenderManager->GetPointLightsCount(); ++i)
      {
         if (!g_RenderManager->CubeShadowMaps[i])
            continue;

         std::stringstream name;
         name << "CubeShadowMaps[" << i << ']';

         ShaderProgram->SetTexture2D(name.str(), g_RenderManager->CubeShadowMaps[i]);
      }
   }

   DebugPrimitiveMaterial::DebugPrimitiveMaterial()
      : BaseMaterial("res/shaders/debug-primitive.vs",
                     "res/shaders/debug-primitive.fs",
                     "res/shaders/debug-primitive.gs")
   {
      ShaderProgram->AddInputBuffer(g_RenderManager->PositionsVBO, 3, 0, sizeof(mm::vec3), Type::Float);
   }

   void DebugPrimitiveMaterial::ResolveUniforms()
   {
      ShaderProgram->SetFloats("LinesColor", Color);
   }
}