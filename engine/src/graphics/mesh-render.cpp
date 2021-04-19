#pragma once
#include "mesh-render.h"
#include "entry-point/global_systems.h"

namespace graphics
{
   BaseMaterial::BaseMaterial(const std::string_view& vertexShaderPath, const std::string_view& fragmentShaderPath)
      : InstanceId(GenerateMaterialId())
   {
      std::string_view vertexShaderSrc;
      std::string_view fragmentShaderSrc;

      if ((vertexShaderSrc = utils::ReadFromFile(vertexShaderPath)) == "")
         PRINT_AND_TERMINATE("Couldn't read vertex shader file: %s", &vertexShaderPath[0]);
      if ((fragmentShaderSrc = utils::ReadFromFile(fragmentShaderPath)) == "")
         PRINT_AND_TERMINATE("Couldn't read fragment shader file: %s", &fragmentShaderPath[0]);

      ShaderProgram = g_GraphicsDevice->CreateShaderProgram();

      ShaderProgram->AddShader(ShaderType::Vertex, vertexShaderSrc);
      ShaderProgram->AddShader(ShaderType::Fragment, fragmentShaderSrc);

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

      ShaderProgram->AddInputBuffer(g_RenderManager->LightsUBO, "LightBlock", sizeof(PointLightAligned16) * MaxPointLights);
   }

   void PhongMaterial::ResolveUniforms()
   {
      ShaderProgram->SetFloats("Diffuse", Diffuse);

      ShaderProgram->SetFloats("Specular", Specular);

      ShaderProgram->SetFloats("Emissive", Emissive);

      ShaderProgram->SetFloat("Glossiness", Glossiness);


      ShaderProgram->SetTexture2D("DiffuseTexture", DiffuseTexture);
      ShaderProgram->SetTexture2D("NormalTexture", NormalTexture);
   }
}