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

      PositionsVBO = GraphicsDevice->CreateVBO();
      PositionsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      NormalsVBO = GraphicsDevice->CreateVBO();
      NormalsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      UVsVBO = GraphicsDevice->CreateVBO();
      UVsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector2), nullptr);

      MainShader->AddInputBuffer(PositionsVBO, 3, PositionAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(NormalsVBO, 3, NormalAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(UVsVBO, 2, UvAttribLocation, sizeof(math::Vector2), Type::Float);
   }

   ForwardRender::~ForwardRender()
   {

   }

   void ForwardRender::Render(const Camera& camera)
   {
      MainShader->Use();

      MainShader->SetFloats("projection", camera.GetCameraProjection());
      MainShader->SetFloats("view", camera.GetCameraViewMatrix());

      for(auto r : RendererList)
      { 
         MainShader->SetFloats("model", r.Transformation);

         PositionsVBO->UpdateData(r.Mesh.Positions.size() * sizeof(math::Vector3), &r.Mesh.Positions[0]);
         NormalsVBO->UpdateData(r.Mesh.Normals.size() * sizeof(math::Vector3), &r.Mesh.Normals[0]);
         UVsVBO->UpdateData(r.Mesh.UVs.size() * sizeof(math::Vector2), &r.Mesh.UVs[0]);

         GraphicsDevice->DrawTriangles(MainShader, r.Mesh.Positions.size() * 3);
      }
   }
}
