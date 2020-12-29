#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "scene/scene.h"

#include "math/matrices/matrix4x4.h"
#include "scene/renders/forward-render.h"

#include "asset-manager/obj-loader.h"

#include "math/math_utils.h"

#include <string.h>
#include <Windows.h>

char g_KeyStates[1024];
Camera* g_MainCamera;

float g_DeltaTime = 1.0f / 60.0f;


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   if (action == GLFW_PRESS)
      g_KeyStates[key] = 1;
   if (action == GLFW_RELEASE)
      g_KeyStates[key] = 0;
}

void CursorCallback(GLFWwindow* window, double posX, double posY)
{
   RotateCamera(g_MainCamera, posX, posY, g_DeltaTime);
}

void InputHandle()
{
   if (g_KeyStates[GLFW_KEY_W])
      MoveCamera(g_MainCamera, CameraMoveForward, g_DeltaTime);
   if(g_KeyStates[GLFW_KEY_S])
      MoveCamera(g_MainCamera, CameraMoveBackward, g_DeltaTime);
   if(g_KeyStates[GLFW_KEY_D])
      MoveCamera(g_MainCamera, CameraMoveRight, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_A])
      MoveCamera(g_MainCamera, CameraMoveLeft, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_E])
      MoveCamera(g_MainCamera, CameraMoveUp, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_Q])
      MoveCamera(g_MainCamera, CameraMoveDown, g_DeltaTime);
}

void GLAPIENTRY
MessageCallback(GLenum source,
   GLenum type,
   GLuint id,
   GLenum severity,
   GLsizei length,
   const GLchar* message,
   const void* userParam)
{
   fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
      (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
      type, severity, message);
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

   glEnable(GL_DEPTH_TEST);

   glfwSetKeyCallback(window, KeyCallback);
   glfwSetCursorPosCallback(window, CursorCallback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(MessageCallback, 0);
   
   g_MainCamera = CreateCamera((Vector3) { 0.0f, 0.0f, 10.0f }, PI / 4, 1.7f, 5.0f);

   Scene* scene = CreateScene(g_MainCamera);

   MeshData pistolMeshData = LoadMesh("res/meshes/pistol/pistol.obj");
   AddRenderObject(scene, CreateRenderObject(CreateMesh(pistolMeshData), NULL, CreateTranslateMatrix((Vector3) { 0.0f, 0.0f, 0.0f })));

   MeshData cubeMeshData = LoadMesh("res/meshes/cube.obj");
   Matrix4 cubeTransform = CreateTranslateMatrix((Vector3) { 0.0f, -2.0f, 0.0f });
   cubeTransform = multiply_matrix(cubeTransform, CreateScaleMatrix((Vector3){5.0f, 1.0f, 5.0f}));
   AddRenderObject(scene, CreateRenderObject(CreateMesh(cubeMeshData), NULL, cubeTransform));
   
   AddPointLight(scene, CreatePointLight((Vector3) { 1.2f, 1.0f, 2.0f }, (Vector3) { 1.0f, 1.0f, 1.0f }, 0.09f, 0.032f, 1.0f));
   
   InitializeForwardRender();

   LARGE_INTEGER freq;
   QueryPerformanceFrequency(&freq);

   while (!glfwWindowShouldClose(window))
   {
      LARGE_INTEGER startFrameTicks;
      QueryPerformanceCounter(&startFrameTicks);


      glfwPollEvents();
      InputHandle();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      UpdateForwardRender(scene, g_DeltaTime);

      glfwSwapBuffers(window);


      LARGE_INTEGER endFrameTicks;
      QueryPerformanceCounter(&endFrameTicks);

      g_DeltaTime = endFrameTicks.QuadPart - startFrameTicks.QuadPart;
      g_DeltaTime /= freq.QuadPart / 1000.0f;
      g_DeltaTime = 1.0f / g_DeltaTime;
   }

   glfwTerminate();
   return 0;
}
