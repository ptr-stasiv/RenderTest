#include "forward-render.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "graphics/shaders/shader-pipeline.h"
#include "utils/read-from-file.h"

namespace graphics
{
   static graphics::ShaderPipeline g_RenderShaderPipeline;

   void ForwardRender::Initialize()
   {
      g_RenderShaderPipeline.Create();

      g_RenderShaderPipeline.Add(graphics::ShaderType::Vertex, utils::ReadFromFile("res/shaders/test-display.vs"));
      g_RenderShaderPipeline.Add(graphics::ShaderType::Fragment, utils::ReadFromFile("res/shaders/test-display.fs"));

      g_RenderShaderPipeline.Compile();
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
      g_RenderShaderPipeline.Use();

      g_RenderShaderPipeline.SetInt("LightsCount", scene.LightsList.size());
      g_RenderShaderPipeline.SetInt("PointLightsCount", scene.PointLightsList.size());
      g_RenderShaderPipeline.SetInt("SpotlightsCount", scene.SpotlightsList.size());

      for (int i = 0; i < scene.LightsList.size(); ++i)
      {
         char* posMemberStr = ArrayMemberToStr("LightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("LightsArray[", i, "].Color");

         g_RenderShaderPipeline.SetFloats(posMemberStr, scene.LightsList.at(i).Position);
         g_RenderShaderPipeline.SetFloats(colorMemberStr, scene.LightsList.at(i).Color);

         free(posMemberStr);
         free(colorMemberStr);
      }

      for (int i = 0; i < scene.PointLightsList.size(); ++i)
      {
         char* posMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Color");
         char* linearMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Linear");
         char* quadraticMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Quadratic");
         char* constantMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Constant");

          g_RenderShaderPipeline.SetFloats(posMemberStr, scene.PointLightsList.at(i).Position);
          g_RenderShaderPipeline.SetFloats(colorMemberStr, scene.PointLightsList.at(i).Color);
          g_RenderShaderPipeline.SetFloat(linearMemberStr, scene.PointLightsList.at(i).Linear);
          g_RenderShaderPipeline.SetFloat(quadraticMemberStr, scene.PointLightsList.at(i).Quadratic);
          g_RenderShaderPipeline.SetFloat(constantMemberStr, scene.PointLightsList.at(i).Constant);

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

          g_RenderShaderPipeline.SetFloats(posMemberStr, scene.SpotlightsList.at(i).Position);
          g_RenderShaderPipeline.SetFloats(directionMemberStr, scene.SpotlightsList.at(i).Direction);
          g_RenderShaderPipeline.SetFloats(colorMemberStr, scene.SpotlightsList.at(i).Color);
          g_RenderShaderPipeline.SetFloat(outerAngleMemberStr, scene.SpotlightsList.at(i).OuterAngle);
          g_RenderShaderPipeline.SetFloat(innerAngleMemberStr, scene.SpotlightsList.at(i).InnerAngle);

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

         g_RenderShaderPipeline.SetFloats(colorMemberStr, scene.MaterialsList.at(i).Color);
         g_RenderShaderPipeline.SetFloats(specMemberStr, scene.MaterialsList.at(i).Specular);
         g_RenderShaderPipeline.SetFloats(emisMemberStr, scene.MaterialsList.at(i).Emissive);
         g_RenderShaderPipeline.SetFloat(expMemberStr, scene.MaterialsList.at(i).ShineExponent);

         free(colorMemberStr);
         free(specMemberStr);
         free(emisMemberStr);
         free(expMemberStr);
      }

      for (int i = 0; i < scene.RenderObjectsList.size(); ++i)
      {
         RenderObject r = scene.RenderObjectsList.at(i);

         g_RenderShaderPipeline.SetFloats("model", r.Transform);
         g_RenderShaderPipeline.SetFloats("view", scene.GetCamera()->GetCameraViewMatrix());
         g_RenderShaderPipeline.SetFloats("projection", scene.GetCamera()->GetCameraProjection());

         g_RenderShaderPipeline.SetInt("MaterialId", r.MaterialRef);

         g_RenderShaderPipeline.SetFloats("CameraPosition", scene.GetCamera()->Position);


         auto diffuseTexture = scene.MaterialsList.at(r.MaterialRef).DiffuseTexture;

         if (diffuseTexture.Id != 0)
         {
            if (LoadedTextureLookup.find(diffuseTexture.Id) != LoadedTextureLookup.end())
            {
               GLuint bindUnit = LoadedTextureLookup.size() % 16;
               glBindTextureUnit(bindUnit, LoadedTextureLookup.at(diffuseTexture.Id));
               g_RenderShaderPipeline.SetInt("DiffuseTexture", bindUnit);
            }
            else
               LoadedTextureLookup[diffuseTexture.Id] = CreateTexture(diffuseTexture);

            g_RenderShaderPipeline.SetInt("UseDiffuseTexture", 1);
         }
         else
            g_RenderShaderPipeline.SetInt("UseDiffuseTexture", 0);

         glBindVertexArray(r.MeshData.VaoId);
         glDrawArrays(GL_TRIANGLES, 0, r.MeshData.VerticesCount);
         glBindVertexArray(0);
      }
   }
}