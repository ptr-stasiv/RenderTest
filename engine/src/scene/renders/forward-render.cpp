#include "forward-render.h"
#include "../../bgl/shader.h"

#include <cstring>
#include <cstdio>
#include <cstdlib>

namespace graphics
{
   void ForwardRender::Initialize()
   {
      char* vsSrc = ReadFromFile("res/shaders/test-display.vs");
      char* fsSrc = ReadFromFile("res/shaders/test-display.fs");

      GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vsSrc);
      GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fsSrc);

      free(vsSrc);
      free(fsSrc);

      RenderShader = glCreateProgram();
      glAttachShader(RenderShader, vertexShader);
      glAttachShader(RenderShader, fragmentShader);
      glLinkProgram(RenderShader);
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

   void ForwardRender::Update(const Scene& scene, const float deltaTime)
   {
      glUseProgram(RenderShader);

      SetShaderInt(RenderShader, "LightsCount", scene.LightsCount);
      SetShaderInt(RenderShader, "PointLightsCount", scene.PointLightsCount);
      SetShaderInt(RenderShader, "SpotlightsCount", scene.SpotlightsCount);

      for (int i = 0; i < scene.LightsCount; ++i)
      {
         char* posMemberStr = ArrayMemberToStr("LightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("LightsArray[", i, "].Color");

         SetShaderVector3(RenderShader, posMemberStr, scene.LightsArray[i].Position);
         SetShaderVector3(RenderShader, colorMemberStr, scene.LightsArray[i].Color);

         free(posMemberStr);
         free(colorMemberStr);
      }

      for (int i = 0; i < scene.PointLightsCount; ++i)
      {
         char* posMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Color");
         char* linearMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Linear");
         char* quadraticMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Quadratic");
         char* constantMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Constant");

         SetShaderVector3(RenderShader, posMemberStr, scene.PointLightsArray[i].Position);
         SetShaderVector3(RenderShader, colorMemberStr, scene.PointLightsArray[i].Color);
         SetShaderFloat(RenderShader, linearMemberStr, scene.PointLightsArray[i].Linear);
         SetShaderFloat(RenderShader, quadraticMemberStr, scene.PointLightsArray[i].Quadratic);
         SetShaderFloat(RenderShader, constantMemberStr, scene.PointLightsArray[i].Constant);

         free(posMemberStr);
         free(colorMemberStr);
         free(linearMemberStr);
         free(quadraticMemberStr);
         free(constantMemberStr);
      }

      for (int i = 0; i < scene.SpotlightsCount; ++i)
      {
         char* posMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Position");
         char* colorMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Color");
         char* directionMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Direction");
         char* outerAngleMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].OuterAngle");
         char* innerAngleMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].InnerAngle");

         SetShaderVector3(RenderShader, posMemberStr, scene.SpotlightsArray[i].Position);
         SetShaderVector3(RenderShader, directionMemberStr, scene.SpotlightsArray[i].Direction);
         SetShaderVector3(RenderShader, colorMemberStr, scene.SpotlightsArray[i].Color);
         SetShaderFloat(RenderShader, outerAngleMemberStr, scene.SpotlightsArray[i].OuterAngle);
         SetShaderFloat(RenderShader, innerAngleMemberStr, scene.SpotlightsArray[i].InnerAngle);

         free(posMemberStr);
         free(colorMemberStr);
         free(directionMemberStr);
         free(outerAngleMemberStr);
         free(innerAngleMemberStr);
      }

      for (int i = 0; i < scene.MaterialsCount; ++i)
      {
         char* colorMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].Color");
         char* specMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].Specular");
         char* emisMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].Emissive");
         char* expMemberStr = ArrayMemberToStr("MaterialsArray[", i, "].ShineExponent");

         SetShaderVector3(RenderShader, colorMemberStr, scene.MaterialsArray[i].Color);
         SetShaderVector3(RenderShader, specMemberStr, scene.MaterialsArray[i].Specular);
         SetShaderVector3(RenderShader, emisMemberStr, scene.MaterialsArray[i].Emissive);
         SetShaderFloat(RenderShader, expMemberStr, scene.MaterialsArray[i].ShineExponent);

         free(colorMemberStr);
         free(specMemberStr);
         free(emisMemberStr);
         free(expMemberStr);
      }

      for (int i = 0; i < scene.RenderObjectCount; ++i)
      {
         RenderObject r = scene.RenderObjectList[i];

         SetShaderMatrix4(RenderShader, "model", r.Transform);
         SetShaderMatrix4(RenderShader, "view", GetCameraViewMatrix(scene.Camera));
         SetShaderMatrix4(RenderShader, "projection", GetCameraProjection(scene.Camera));

         SetShaderInt(RenderShader, "MaterialId", r.MaterialRef);

         SetShaderVector3(RenderShader, "CameraPosition", scene.Camera->Position);


         auto diffuseTexture = scene.MaterialsArray[r.MaterialRef].DiffuseTexture;

         if (diffuseTexture.Id != 0)
         {
            if (LoadedTextureLookup.find(diffuseTexture.Id) != LoadedTextureLookup.end())
            {
               GLuint bindUnit = LoadedTextureLookup.size() % 16;
               glBindTextureUnit(bindUnit, LoadedTextureLookup.at(diffuseTexture.Id));
               SetShaderInt(RenderShader, "DiffuseTexture", bindUnit);
            }
            else
               LoadedTextureLookup[diffuseTexture.Id] = CreateTexture(diffuseTexture);

            SetShaderInt(RenderShader, "UseDiffuseTexture", 1);
         }
         else
            SetShaderInt(RenderShader, "UseDiffuseTexture", 0);

         glBindVertexArray(r.MeshData->VaoId);
         glDrawArrays(GL_TRIANGLES, 0, r.MeshData->VerticesCount);
         glBindVertexArray(0);
      }
   }
}