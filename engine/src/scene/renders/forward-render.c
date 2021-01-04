#include "forward-render.h"
#include "../../bgl/shader.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static GLuint g_MainShaderProgram;

void InitializeForwardRender()
{
   char* vsSrc = ReadFromFile("res/shaders/test-display.vs");
   char* fsSrc = ReadFromFile("res/shaders/test-display.fs");

   GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vsSrc);
   GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fsSrc);

   free(vsSrc);
   free(fsSrc);

   g_MainShaderProgram = glCreateProgram();
   glAttachShader(g_MainShaderProgram, vertexShader);
   glAttachShader(g_MainShaderProgram, fragmentShader);
   glLinkProgram(g_MainShaderProgram);
}

char* ArrayMemberToStr(const char* arrStr, const int index, const char* memberStr)
{
   char indexStr[32];
   itoa(index, indexStr, 10);

   int arrStrSize    = strlen(arrStr);
   int indexStrSize  = strlen(indexStr);
   int memberStrSize = strlen(memberStr);

   char* res = calloc(arrStrSize + indexStrSize + memberStrSize + 1, 1);
   
   strcat(res, arrStr);
   strcat(res + arrStrSize, indexStr);
   strcat(res + indexStrSize, memberStr);

   return res;
}

void UpdateForwardRender(Scene* scene, const float deltaTime)
{
   glUseProgram(g_MainShaderProgram);

   SetShaderInt(g_MainShaderProgram, "LightsCount", scene->LightsCount);
   SetShaderInt(g_MainShaderProgram, "PointLightsCount", scene->PointLightsCount);
   SetShaderInt(g_MainShaderProgram, "SpotlightsCount", scene->SpotlightsCount);

   for (int i = 0; i < scene->LightsCount; ++i)
   {
      char* posMemberStr = ArrayMemberToStr("LightsArray[", i, "].Position");
      char* colorMemberStr = ArrayMemberToStr("LightsArray[", i, "].Color");

      SetShaderVector3(g_MainShaderProgram, posMemberStr, scene->LightsArray[i].Position);
      SetShaderVector3(g_MainShaderProgram, colorMemberStr, scene->LightsArray[i].Color);

      free(posMemberStr);
      free(colorMemberStr);
   }

   for (int i = 0; i < scene->PointLightsCount; ++i)
   {
      char* posMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Position");
      char* colorMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Color");
      char* linearMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Linear");
      char* quadraticMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Quadratic");
      char* constantMemberStr = ArrayMemberToStr("PointLightsArray[", i, "].Constant");

      SetShaderVector3(g_MainShaderProgram, posMemberStr, scene->PointLightsArray[i].Position);
      SetShaderVector3(g_MainShaderProgram, colorMemberStr, scene->PointLightsArray[i].Color);
      SetShaderFloat(g_MainShaderProgram, linearMemberStr, scene->PointLightsArray[i].Linear);
      SetShaderFloat(g_MainShaderProgram, quadraticMemberStr, scene->PointLightsArray[i].Quadratic);
      SetShaderFloat(g_MainShaderProgram, constantMemberStr, scene->PointLightsArray[i].Constant);

      free(posMemberStr);
      free(colorMemberStr);
      free(linearMemberStr);
      free(quadraticMemberStr);
      free(constantMemberStr);
   }

   for (int i = 0; i < scene->SpotlightsCount; ++i)
   {
      char* posMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Position");
      char* colorMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Color");
      char* directionMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].Direction");
      char* outerAngleMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].OuterAngle");
      char* innerAngleMemberStr = ArrayMemberToStr("SpotlightsArray[", i, "].InnerAngle");

      SetShaderVector3(g_MainShaderProgram, posMemberStr, scene->SpotlightsArray[i].Position);
      SetShaderVector3(g_MainShaderProgram, directionMemberStr, scene->SpotlightsArray[i].Direction);
      SetShaderVector3(g_MainShaderProgram, colorMemberStr, scene->SpotlightsArray[i].Color);
      SetShaderFloat(g_MainShaderProgram, outerAngleMemberStr, scene->SpotlightsArray[i].OuterAngle);
      SetShaderFloat(g_MainShaderProgram, innerAngleMemberStr, scene->SpotlightsArray[i].InnerAngle);

      free(posMemberStr);
      free(colorMemberStr);
      free(directionMemberStr);
      free(outerAngleMemberStr);
      free(innerAngleMemberStr);
   }

   for (int i = 0; i < scene->RenderObjectCount; ++i)
   {
      RenderObject r = scene->RenderObjectList[i];

      SetShaderMatrix4(g_MainShaderProgram, "model", r.Transform);
      SetShaderMatrix4(g_MainShaderProgram, "view", GetCameraViewMatrix(scene->Camera));
      SetShaderMatrix4(g_MainShaderProgram, "projection", GetCameraProjection(scene->Camera));

      SetShaderVector3(g_MainShaderProgram, "CameraPosition", scene->Camera->Position);
      
      glBindVertexArray(r.MeshData->VaoId);
      glDrawArrays(GL_TRIANGLES, 0, r.MeshData->VerticesCount);
      glBindVertexArray(0);
   }
}
