#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

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

typedef struct MeshDataStructure
{
   GLuint VaoId;
   int VerticesCount;
} Mesh;

Mesh* CreateMesh(float* vertices, const int verticesCount)
{
   Mesh* resMesh = malloc(sizeof(Mesh));
   if (!resMesh)
      return NULL;

   resMesh->VerticesCount = verticesCount;

   GLuint vbo;
   
   glGenVertexArrays(1, &resMesh->VaoId);
   glGenBuffers(1, &vbo);

   glBindVertexArray(resMesh->VaoId);
   glBindBuffer(GL_ARRAY_BUFFER, vbo);

   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesCount, vertices, GL_STATIC_DRAW);

   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);

   return resMesh;
}

typedef struct TextureDataStructure
{
   char* Data;
   int Width;
   int Height;
} Texture;


typedef struct RenderSceneObjectStructure
{
   Mesh* MeshData;
   Texture* TextureData;
} RenderObject;

RenderObject* CreateRenderObject(Mesh* mesh, Texture* texture)
{
   RenderObject* res = malloc(sizeof(RenderObject));
   if (!res)
      return NULL;

   res->MeshData = mesh;
   res->TextureData = texture;

   return res;
}


#define MAX_SCENE_OBJECTS 32

typedef struct SceneStructure
{
   RenderObject** RenderObjectList;
   int RenderObjectCount;
} Scene;

Scene* CreateScene()
{
   Scene* res = malloc(sizeof(Scene));
   if (!res)
      return NULL;

   res->RenderObjectList = malloc(sizeof(RenderObject) * MAX_SCENE_OBJECTS);
   res->RenderObjectCount = 0;
}

void AddRenderObject(Scene* scene, RenderObject* renderObject)
{
   if (scene->RenderObjectCount >= MAX_SCENE_OBJECTS)
      return;

   scene->RenderObjectList[scene->RenderObjectCount] = renderObject;
   scene->RenderObjectCount++;
}


void RenderScene(Scene* scene)
{
   for (int i = 0; i < scene->RenderObjectCount; ++i)
   {
      RenderObject* r = scene->RenderObjectList[i];

      glBindVertexArray(r->MeshData->VaoId);

      glDrawArrays(GL_TRIANGLES, 0, r->MeshData->VerticesCount);

      glBindVertexArray(0);
   }
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