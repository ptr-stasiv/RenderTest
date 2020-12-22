#include "forward-render.h"
#include "../../bgl/shader.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static GLuint g_MainShaderProgram;

void InitializeForwardRender()
{
   char* vsSrc = ReadFile("res/shaders/test-display.vs");
   char* fsSrc = ReadFile("res/shaders/test-display.fs");

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

void UpdateForwardRender(Scene* scene)
{
   glUseProgram(g_MainShaderProgram);

   SetShaderInt(g_MainShaderProgram, "LightsCount", scene->LightsCount);

   for (int i = 0; i < scene->LightsCount; ++i)
   {
      char* posMemberStr = ArrayMemberToStr("LightsArray[", i, "].Position");
      char* colorMemberStr = ArrayMemberToStr("LightsArray[", i, "].Color");

      SetShaderVector3(g_MainShaderProgram, posMemberStr, scene->LightsArray[i].Position);
      SetShaderVector3(g_MainShaderProgram, colorMemberStr, scene->LightsArray[i].Color);

      free(posMemberStr);
      free(colorMemberStr);
   }

   for (int i = 0; i < scene->RenderObjectCount; ++i)
   {
      RenderObject r = *scene->RenderObjectList[i];

      SetShaderMatrix4(g_MainShaderProgram, "model", r.Transform);
      SetShaderMatrix4(g_MainShaderProgram, "view", GetCameraViewMatrix(scene->Camera));
      SetShaderMatrix4(g_MainShaderProgram, "projection", GetCameraProjection(scene->Camera));

      SetShaderVector3(g_MainShaderProgram, "CameraPosition", scene->Camera->Position);
      
      glBindVertexArray(r.MeshData->VaoId);
      glDrawArrays(GL_TRIANGLES, 0, r.MeshData->VerticesCount);
      glBindVertexArray(0);
   }
}
