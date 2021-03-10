#include "forward-render.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "graphics/shaders/shader-pipeline.h"
#include "utils/read-from-file.h"
#include "platforms/opengl/primitives/buffers/vertex-array.h"
#include "platforms/opengl/primitives/textures/texture2d.h"
#include "graphics/shaders/compute.h"

#include "vendors/glm/glm.hpp"
#include "vendors/glm/gtc/matrix_transform.hpp"

namespace graphics
{
   static std::unique_ptr<graphics::ShaderPipeline> g_RenderShaderPipeline;

   struct Tile
   {
      math::Vector2 Position;
      math::Vector2 Size;
   };

   struct Sphere
   {
      math::Vector3 Center;
      float Radius;
   };

   static std::vector<Sphere> SphereList;

   static constexpr uint8_t tilesX = 32;
   static constexpr uint8_t tilesY = 32;

   static Tile Tiles[tilesX * tilesY];

   static gl::Texture2D TileStatesTexture;
   
   static gl::VertexArray TilesVao;

   static std::unique_ptr<graphics::ShaderPipeline> TileShader;

   void ForwardRender::Initialize()
   {
      g_RenderShaderPipeline = std::make_unique<graphics::ShaderPipeline>();

      g_RenderShaderPipeline->Add(graphics::ShaderType::Vertex, utils::ReadFromFile("res/shaders/test-display.vs"));
      g_RenderShaderPipeline->Add(graphics::ShaderType::Fragment, utils::ReadFromFile("res/shaders/test-display.fs"));

      g_RenderShaderPipeline->Compile();


      const char* vertexShaderSrc = R"(
         #version 460 core
      
         layout(location = 0) in vec2 pos;

         out vec2 UV;
      
         void main()
         {
            float u = pos.x / 2.0f + 0.5f;
            float v = pos.y / 2.0f + 0.5f;

            UV = vec2(u, v);
            gl_Position = vec4(pos.xy, 0.0f, 1.0f);
         })";
      
      const char* fragmentShaderSrc = R"(
         #version 460 core
      
         out vec4 FragOutput;
      
         uniform sampler2D Texture;

         in vec2 UV;
      
         void main()
         {
            FragOutput = texture(Texture, UV);
         })";
      
      TileShader = std::make_unique<graphics::ShaderPipeline>();
      
      TileShader->Add(graphics::ShaderType::Vertex, vertexShaderSrc);
      TileShader->Add(graphics::ShaderType::Fragment, fragmentShaderSrc);
      
      TileShader->Compile();

      auto csSrc = R"(
         #version 460 core

         layout(local_size_x = 1) in;
         
         uniform float WindowAR;
         
         struct Tile
         {
            vec2 Position;   
            vec2 Size;   
         };
         
         layout(binding = 0, std430) buffer shaderData
         {
            Tile TileArray[];
         };

         void main()
         {
            float tileSizeX = 2.0f / gl_NumWorkGroups.x;
            float tileSizeY = 2.0f / gl_NumWorkGroups.y;

            float x = -1.0f + tileSizeX * gl_GlobalInvocationID.x;  
            float y = 1.0f - tileSizeY * gl_GlobalInvocationID.y;
            
            Tile newTile;
            newTile.Position = vec2(x, y);
            newTile.Size = vec2(tileSizeX, tileSizeY);

            TileArray[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x]= newTile;
         })";

      graphics::ComputeShader cs(csSrc);

      cs.Use();

      cs.SetFloat("WindowAR", 1.77f);

      GLuint ssbo;
      glGenBuffers(1, &ssbo);
      glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
      glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Tiles), nullptr, GL_DYNAMIC_DRAW);
      glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

      cs.Dispatch(tilesX, tilesY);
      glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

      glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
      glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Tiles), &Tiles[0]);


      TilesVao = gl::CreateVertexArray();

      float positions[] =
      {
         -1.0f, -1.0f,
         -1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, -1.0f,
         -1.0f, -1.0f
      };

      gl::VertexBuffer vbo = gl::CreateVertexBuffer(sizeof(positions), positions);
      GLuint b = gl::AddBufferVertexArray(TilesVao, vbo, 2 * sizeof(float));
      gl::AddAttribFormatVertexArray(TilesVao, 0, b, 2, GL_FLOAT, GL_FALSE, 0);

      gl::TextureParams tParams;
      tParams.MagFilter = GL_NEAREST;
      tParams.MinFilter = GL_NEAREST;
      tParams.WrapS = GL_CLAMP_TO_EDGE;
      tParams.WrapT = GL_CLAMP_TO_EDGE;

      TileStatesTexture = gl::CreateTexture2D(tilesX, tilesY, GL_RGBA8, GL_RGBA, GL_FLOAT, tParams);
   }

   char* ArrayMemberToStr(const char* arrStr, const int index, const char* memberStr)
   {
      char indexStr[32];
      itoa(index, indexStr, 10);

      int arrStrSize = strlen(arrStr);
      int indexStrSize = strlen(indexStr);
      int memberStrSize = strlen(memberStr);

      char* res = (char*)calloc(arrStrSize + indexStrSize + memberStrSize + 1, 1);

      strcat(res, arrStr);
      strcat(res + arrStrSize, indexStr);
      strcat(res + indexStrSize, memberStr);

      return res;
   }

   GLuint CreateTexture(const assets::AssetRef& assetRef)
   {
      auto assetData = assetRef.GetData<assets::ImageAssetData>();

      GLenum format = assetData->Channels == 4 ? GL_RGBA : GL_RGB;
      GLenum internalFormat = assetData->Channels == 4 ? GL_RGBA8 : GL_RGB8;

      GLuint resTex;
      glCreateTextures(GL_TEXTURE_2D, 1, &resTex);
      glTextureStorage2D(resTex, 1, internalFormat, assetData->Width, assetData->Height);
      glTextureSubImage2D(resTex, 0, 0, 0, assetData->Width, assetData->Height, format, GL_UNSIGNED_BYTE, assetData->Pixels);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      return resTex;
   }

   void ForwardRender::Update(const core::Scene& scene, const float deltaTime)
   {
      g_RenderShaderPipeline->Use();

      g_RenderShaderPipeline->SetInt("LightsCount", scene.LightsList.size());
      g_RenderShaderPipeline->SetInt("PointLightsCount", scene.PointLightsList.size());
      g_RenderShaderPipeline->SetInt("SpotlightsCount", scene.SpotlightsList.size());

      for (int i = 0; i < scene.LightsList.size(); ++i)
      {
         char* posMemberStr = ArrayMemberToStr("LightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("LightsArray[", i, "].Color");

         g_RenderShaderPipeline->SetFloats(posMemberStr, scene.LightsList.at(i).Position);
         g_RenderShaderPipeline->SetFloats(colorMemberStr, scene.LightsList.at(i).Color);

         free(posMemberStr);
         free(colorMemberStr);
      }

      debug::DebugDrawManager debugManager;

      for (int i = 0; i < scene.PointLightsList.size(); ++i)
      {
         char* posMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Color");
         char* linearMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Linear");
         char* quadraticMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Quadratic");
         char* constantMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Constant");

         g_RenderShaderPipeline->SetFloats(posMemberStr, scene.PointLightsList.at(i).Position);
         g_RenderShaderPipeline->SetFloats(colorMemberStr, scene.PointLightsList.at(i).Color);
         g_RenderShaderPipeline->SetFloat(linearMemberStr, scene.PointLightsList.at(i).Linear);
         g_RenderShaderPipeline->SetFloat(quadraticMemberStr, scene.PointLightsList.at(i).Quadratic);
         g_RenderShaderPipeline->SetFloat(constantMemberStr, scene.PointLightsList.at(i).Constant);

         auto pos = scene.PointLightsList.at(i).Position;
         auto linear = scene.PointLightsList.at(i).Linear;
         auto quad = scene.PointLightsList.at(i).Quadratic;
         auto con = scene.PointLightsList.at(i).Constant;


         //Test for the tile rendering
         //Compute the point light influence radius
         //This can be computed by resolving 1 / (constant + linear * lightDist + quadratic * lightDist * lightDist) > n
         //Where n is the number which define what influence should light have to not being cull for specific fragment

         auto d = (linear * linear) - 4 * quad * (con - 10.0f); //Here 10.0f mean the minimum brightness of 0.1f
         auto r = (-linear + std::sqrt(d)) / (2 * quad);

         debugManager.AddDebugSphere(scene.PointLightsList.at(i).Position, r, 12, 12);

         glm::vec4 posg(scene.PointLightsList.at(i).Position.x, scene.PointLightsList.at(i).Position.y, scene.PointLightsList.at(i).Position.z, 1.0f);
         glm::mat4 proj = glm::perspective(scene.GetCamera()->Fov, scene.GetCamera()->Aspect, 0.1f, 1000.0f);
         glm::mat4 view = glm::lookAt(glm::vec3(scene.GetCamera()->Position.x, scene.GetCamera()->Position.y, scene.GetCamera()->Position.z + 1.0f), glm::vec3(scene.GetCamera()->Position.x, scene.GetCamera()->Position.y, scene.GetCamera()->Position.z), glm::vec3(scene.GetCamera()->UpAxis.x, scene.GetCamera()->UpAxis.y, scene.GetCamera()->UpAxis.z));

         glm::vec4 clips = proj * (posg * view);
         glm::vec4 clipsR = proj * ((posg + r) * view);
         math::Vector3 ndc = math::Vector3(clips.x / clips.w, clips.y / clips.w, clips.z / clips.w);
         math::Vector3 ndcR = math::Vector3(clipsR.x / clipsR.w, clipsR.y / clipsR.w, clipsR.z / clipsR.w);

         SphereList.push_back({ndc, math::Length(ndcR - ndc)});

         free(posMemberStr);
         free(colorMemberStr);
         free(linearMemberStr);
         free(quadraticMemberStr);
         free(constantMemberStr);
      }

      for (int i = 0; i < scene.SpotlightsList.size(); ++i)
      {
         char* posMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Color");
         char* directionMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Direction");
         char* outerAngleMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].OuterAngle");
         char* innerAngleMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].InnerAngle");

         g_RenderShaderPipeline->SetFloats(posMemberStr, scene.SpotlightsList.at(i).Position);
         g_RenderShaderPipeline->SetFloats(directionMemberStr, scene.SpotlightsList.at(i).Direction);
         g_RenderShaderPipeline->SetFloats(colorMemberStr, scene.SpotlightsList.at(i).Color);
         g_RenderShaderPipeline->SetFloat(outerAngleMemberStr, scene.SpotlightsList.at(i).OuterAngle);
         g_RenderShaderPipeline->SetFloat(innerAngleMemberStr, scene.SpotlightsList.at(i).InnerAngle);

         free(posMemberStr);
         free(colorMemberStr);
         free(directionMemberStr);
         free(outerAngleMemberStr);
         free(innerAngleMemberStr);
      }

      for (int i = 0; i < scene.MaterialsList.size(); ++i)
      {
         char* colorMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].Color");
         char* specMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].Specular");
         char* emisMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].Emissive");
         char* expMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].ShineExponent");

         g_RenderShaderPipeline->SetFloats(colorMemberStr, scene.MaterialsList.at(i).Color);
         g_RenderShaderPipeline->SetFloats(specMemberStr, scene.MaterialsList.at(i).Specular);
         g_RenderShaderPipeline->SetFloats(emisMemberStr, scene.MaterialsList.at(i).Emissive);
         g_RenderShaderPipeline->SetFloat(expMemberStr, scene.MaterialsList.at(i).ShineExponent);

         free(colorMemberStr);
         free(specMemberStr);
         free(emisMemberStr);
         free(expMemberStr);
      }

      for (int i = 0; i < scene.RenderObjectsList.size(); ++i)
      {
         RenderObject r = scene.RenderObjectsList.at(i);

         g_RenderShaderPipeline->SetFloats("model", r.Transform);
         g_RenderShaderPipeline->SetFloats("view", scene.GetCamera()->GetCameraViewMatrix());
         g_RenderShaderPipeline->SetFloats("projection", scene.GetCamera()->GetCameraProjection());

         g_RenderShaderPipeline->SetInt("MaterialId", r.MaterialRef);

         g_RenderShaderPipeline->SetFloats("CameraPosition", scene.GetCamera()->Position);


         auto diffuseTexture = scene.MaterialsList.at(r.MaterialRef).DiffuseTexture;

         if (diffuseTexture.Id != 0)
         {
            if (LoadedTextureLookup.find(diffuseTexture.Id) != LoadedTextureLookup.end())
            {
               GLuint bindUnit = LoadedTextureLookup.size() % 16;
               glBindTextureUnit(bindUnit, LoadedTextureLookup.at(diffuseTexture.Id));
               g_RenderShaderPipeline->SetInt("DiffuseTexture", bindUnit);
            }
            else
               LoadedTextureLookup[diffuseTexture.Id] = CreateTexture(diffuseTexture);

            g_RenderShaderPipeline->SetInt("UseDiffuseTexture", 1);
         }
         else
            g_RenderShaderPipeline->SetInt("UseDiffuseTexture", 0);

         glBindVertexArray(r.MeshData.VaoId);
         glDrawArrays(GL_TRIANGLES, 0, r.MeshData.VerticesCount);
         glBindVertexArray(0);
      }

      debugManager.Draw(*scene.GetCamera());

      TileShader->Use();

      math::Vector4 pixels[tilesX * tilesY * 4];

      LOG_ERROR("%f, %f, %f", SphereList[0].Center.x, SphereList[0].Center.y, SphereList[0].Center.z);

      SphereList.clear();

      for (size_t y = 0; y < tilesY; ++y)
      {
         for (size_t x = 0; x < tilesX; ++x)
         {
            if (Tiles[y + x * tilesX].Position.x < 0.5f
               && Tiles[y + x * tilesX].Position.x > -0.5f)
            {
               pixels[y + x * tilesX] = math::Vector4(1.0f, 0.2f, 0.2f, 0.5f);
            }
            else
               pixels[y + x * tilesX] = math::Vector4(0.5f);
         }
      }

      gl::UpdateTexture2D(TileStatesTexture, tilesX, tilesY, pixels);

      gl::BindTexture2D(TileStatesTexture, 0);
      TileShader->SetInt("Texture", 0);

      glBindVertexArray(TilesVao.BindId);
      glDrawArrays(GL_TRIANGLES, 0, tilesX * tilesY * 12);
   }
}
