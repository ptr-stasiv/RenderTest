#include "forward-render.h"

#include <array>

#include "utils/read-from-file.h"

#include "platforms/opengl/gl-shader.h"
#include "platforms/opengl/gl-texture2d.h"

#include "asset-manager/asset-manager.h"

namespace graphics
{
   constexpr std::string_view VertexShaderLocation = "res/shaders/phong-shading.vs";
   constexpr std::string_view FragmentShaderLocation = "res/shaders/phong-shading.fs";

   ForwardRender::ForwardRender(const std::shared_ptr<graphics::GraphicsDevice>& device)
         : GraphicsDevice(device)
   {  
      //MainShader = GraphicsDevice->CreateShaderProgram();
      //MainShader->AddShader(ShaderType::Vertex, utils::ReadFromFile(VertexShaderLocation));
      //MainShader->AddShader(ShaderType::Fragment, utils::ReadFromFile(FragmentShaderLocation));
      //MainShader->Compile();

      
      //VBO's setup 

      PositionsVBO = GraphicsDevice->CreateVBO();
      PositionsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      NormalsVBO = GraphicsDevice->CreateVBO();
      NormalsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      UVsVBO = GraphicsDevice->CreateVBO();
      UVsVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector2), nullptr);

      TangentVBO = GraphicsDevice->CreateVBO();
      TangentVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      BitangentVBO = GraphicsDevice->CreateVBO();
      BitangentVBO->InitData(MaxVerticesPerDraw * sizeof(math::Vector3), nullptr);

      MainShader->AddInputBuffer(PositionsVBO, 3, PositionAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(NormalsVBO, 3, NormalAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(UVsVBO, 2, UvAttribLocation, sizeof(math::Vector2), Type::Float);
      MainShader->AddInputBuffer(TangentVBO, 3, TangentAttribLocation, sizeof(math::Vector3), Type::Float);
      MainShader->AddInputBuffer(BitangentVBO, 3, BitangentAttribLocation, sizeof(math::Vector3), Type::Float);

      //UBO's setup

      LightUBO = GraphicsDevice->CreateUBO();

      LightUBO->InitData((sizeof(PointLightA) + sizeof(SpotlightA)) * (MaxPointLights + MaxSpotlights), nullptr);

      MainShader->AddInputBuffer(LightUBO, "LightBlock", sizeof(PointLightA) * MaxPointLights);
   }

   void ForwardRender::UpdateLight()
   {
      /*MainShader->SetInt("PointLightsCount", PointLightCounter);
      MainShader->SetInt("SpotlightsCount", SpotlightCounter);*/
      
      LightUBO->UpdateData(sizeof(PointLightA) * PointLightCounter, PointLightList);
      LightUBO->UpdateData(sizeof(SpotlightA) * SpotlightCounter, SpotlightList, sizeof(PointLightA) * MaxPointLights); 
   }

   void ForwardRender::ResolveTextures(const BaseMaterial& material)
   {
      //The element organization of two next arrays shouldn't be changed aparat of each other

      //static std::string_view  textureUniformLookup[] =
      //{
      //   "DiffuseTexture",
      //   "SpecularTexture",
      //   "NormalTexture",
      //   "EmissiveTexture",
      //   "GlossinessTexture",
      //};

      //std::array<assets::PixelsData, 5> textureArray =
      //{
      //  material.DiffuseTexture,
      //  material.SpecularTexture,
      //  material.NormalTexture,
      //  material.EmissiveTexture,
      //  material.GlossinessTexture
      //};

      //for (size_t i = 0; i < textureArray.size(); ++i)
      //{
      //   auto t = textureArray[i];
      //   if (!t.IsValid)
      //      continue;

      //   auto foundTexture = TextureLookup.find(t.HashedName);
      //   if (foundTexture == TextureLookup.end())
      //   {
      //      auto texture = GraphicsDevice->CreateTexture2D();

      //      graphics::TextureParams params;
      //      params.MagFilter = graphics::TextureFilter::Linear;
      //      params.MinFilter = graphics::TextureFilter::Nearest;
      //      params.WrapS = graphics::TextureWrap::ClampToEdge;
      //      params.WrapT = graphics::TextureWrap::ClampToEdge;

      //      //This information shoould be later obtained from the assets manager
      //      texture->InitData(t.Width, t.Height,
      //                        graphics::InternalFormat::RGB8, graphics::Format::RGB,
      //                        graphics::Type::Ubyte, params);
      //      texture->UpdateData(t.Width, t.Height, t.Pixels);

      //      TextureLookup[t.HashedName] = texture;
      //   }

      //   MainShader->SetTexture2D(textureUniformLookup[i], TextureLookup.at(t.HashedName));
      //}
   }

   void ForwardRender::Render(const Camera& camera)
   {
      UpdateLight();

      for(auto& mesh : MeshList)
      { 
         auto& transformmatrix = math::CreateScaleMatrix(mesh.Scale)
                                 * math::CreateScaleMatrix(mesh.Rotation)
                                 * math::CreateTranslateMatrix(mesh.Translate);

         auto& material = mesh.Material;
         material->Bind();

         material->SetCameraPosition(camera.Position);
         material->SetObjectToWorldMatrix(transformmatrix);
         material->SetWorldToCameraMatrix(camera.GetCameraViewMatrix());
         material->SetCameraToClipMatrix(camera.GetCameraProjection());

         material->ResolveUniforms();

         PositionsVBO->UpdateData(mesh.Vertices.Positions.size() * sizeof(math::Vector3), &mesh.Vertices.Positions[0]);
         NormalsVBO->UpdateData(mesh.Vertices.Normals.size() * sizeof(math::Vector3), &mesh.Vertices.Normals[0]);
         UVsVBO->UpdateData(mesh.Vertices.UVs.size() * sizeof(math::Vector2), &mesh.Vertices.UVs[0]);
         TangentVBO->UpdateData(mesh.Vertices.Tangents.size() * sizeof(math::Vector3), &mesh.Vertices.Tangents[0]);

         GraphicsDevice->DrawTriangles(MainShader, mesh.Vertices.Positions.size() * 3);
      }
   }
}
