#include "forward-render.h"
#include "../../bgl/shader.h"

static GLuint g_MainShaderProgram;

void InitializeForwardRender()
{
   GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, ReadFile("res/shaders/test-display.vs"));
   GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, ReadFile("res/shaders/test-display.fs"));

   g_MainShaderProgram = glCreateProgram();
   glAttachShader(g_MainShaderProgram, vertexShader);
   glAttachShader(g_MainShaderProgram, fragmentShader);
   glLinkProgram(g_MainShaderProgram);
}

void UpdateForwardRender(Scene* scene)
{
   glUseProgram(g_MainShaderProgram);

   for (int i = 0; i < scene->RenderObjectCount; ++i)
   {
      RenderObject* r = scene->RenderObjectList[i];

      SetShaderMatrix4(g_MainShaderProgram, "model", r->Transform);
      SetShaderMatrix4(g_MainShaderProgram, "view", GetCameraViewMatrix(scene->Camera));
      SetShaderMatrix4(g_MainShaderProgram, "projection", GetCameraProjection(scene->Camera));

      SetShaderVector3(g_MainShaderProgram, "CameraPosition", scene->Camera->Position);
      
      glBindVertexArray(r->MeshData->VaoId);
      glDrawArrays(GL_TRIANGLES, 0, r->MeshData->VerticesCount);
      glBindVertexArray(0);
   }
}
