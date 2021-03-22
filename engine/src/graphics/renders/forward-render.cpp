#include "forward-render.h"

#include <array>

#include "utils/read-from-file.h"

#include "platforms/opengl/gl-shader.h"
#include "platforms/opengl/gl-texture2d.h"

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

   void ForwardRender::ResolveTextures(const Material& material)
   {
      //The element organization of two next arrays shouldn't be changed aparat of each other

      static std::string_view  textureUniformLookup[] =
      {
         "DiffuseTexture",
         "SpecularTexture",
         "NormalTexture",
         "EmissiveTexture",
         "GlossinessTexture",
      };

      std::array<assets::ImageAssetData, 5> textureArray =
      {
        material.DiffuseTexture,
        material.SpecularTexture,
        material.NormalTexture,
        material.EmissiveTexture,
        material.GlossinessTexture
      };

      for (size_t i = 0; i < textureArray.size(); ++i)
      {
         auto t = textureArray[i];
         if (!t.Info.IsValid)
            continue;

         auto foundTexture = TextureLookup.find(t.Info.HashedName);
         if (foundTexture == TextureLookup.end())
         {
            auto texture = GraphicsDevice->CreateTexture2D();

            graphics::TextureParams params;
            params.MagFilter = graphics::TextureFilter::Linear;
            params.MinFilter = graphics::TextureFilter::Nearest;
            params.WrapS = graphics::TextureWrap::ClampToEdge;
            params.WrapT = graphics::TextureWrap::ClampToEdge;

            //This information shoould be later obtained from the assets manager
            texture->InitData(t.Width, t.Height,
                              graphics::InternalFormat::RGB8, graphics::Format::RGB,
                              graphics::Type::Ubyte, params);
            texture->UpdateData(t.Width, t.Height, t.Pixels);

            TextureLookup[t.Info.HashedName] = texture;
         }

         MainShader->SetTexture2D(textureUniformLookup[i], TextureLookup.at(t.Info.HashedName));
      }
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

         ResolveTextures(r.Material);

         MainShader->SetFloats("Model", r.Transformation);

         PositionsVBO->UpdateData(r.Mesh.Positions.size() * sizeof(math::Vector3), &r.Mesh.Positions[0]);
         NormalsVBO->UpdateData(r.Mesh.Normals.size() * sizeof(math::Vector3), &r.Mesh.Normals[0]);
         UVsVBO->UpdateData(r.Mesh.UVs.size() * sizeof(math::Vector2), &r.Mesh.UVs[0]);

         GraphicsDevice->DrawTriangles(MainShader, r.Mesh.Positions.size() * 3);
      }
   }
}
