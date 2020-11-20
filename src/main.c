#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "scene/scene.h"

#include "math/matrices/matrix4x4.h"

char* ReadFile(const char* filepath)
{
   FILE* file = fopen(filepath, "rb");
   if (!file) //Replace with assert
      return NULL;

   fseek(file, 0, SEEK_END);
   long count = ftell(file);
   rewind(file);

   char* buffer = malloc(count); 
   if (!buffer) //Replace with assert
      return NULL;

   fread(buffer, 1, count, file);
   buffer[count] = '\0';

   fclose(file);

   return buffer;
}

GLuint CreateShader(const GLenum shaderType, const char* shaderSrc)
{
   GLuint resShader = glCreateShader(shaderType);
   glShaderSource(resShader, 1, &shaderSrc, NULL);
   glCompileShader(resShader);

   GLint errCode;

   glGetShaderiv(resShader, GL_COMPILE_STATUS, &errCode);
   if (!errCode)
   {
      char log[512];
      glGetShaderInfoLog(resShader, 512, NULL, log);
      printf(log);
   }

   return resShader;
}

int main()
{
   if (!glfwInit())
      return -1;

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

   GLFWwindow* window = glfwCreateWindow(1280, 720, "RenderTest", NULL, NULL);
   if (!window)
      return -1;

   glfwMakeContextCurrent(window);

   if (glewInit() != GLEW_OK)
      return -1;

   Matrix4 m1 = CreateIdentityMatrix4();
   Matrix4 m2 = CreateTranslateMatrix((Vector3){ 5.0f, 3.0f, 7.0f });

   Vector4 pos = (Vector4){10.0f, 6.0f, 10.0f, 1.0f};

   pos = multiply_vmatrix4(pos, m1);

   const char* vertexShaderSrc = ReadFile("res/shaders/test-display.vs");
   const char* fragmentShaderSrc = ReadFile("res/shaders/test-display.fs");

   GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSrc);
   GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

   GLuint shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, vertexShader);
   glAttachShader(shaderProgram, fragmentShader);
   glLinkProgram(shaderProgram);

   float quadVertices[] =
   {
      -0.5f, -0.5f,
      -0.25f, 0.5f,
      0.5f, -0.5f
   };

   Scene* scene = CreateScene();
   AddRenderObject(scene, CreateRenderObject(CreateMesh(quadVertices, 6), NULL));

   glUseProgram(shaderProgram);

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);

      RenderScene(scene);

      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}