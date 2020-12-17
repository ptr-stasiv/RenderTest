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

char g_KeyStates[1024];
Camera* g_MainCamera;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
   if (action == GLFW_PRESS)
      g_KeyStates[key] = 1;
   if (action == GLFW_RELEASE)
      g_KeyStates[key] = 0;
}

void CursorCallback(GLFWwindow* window, double posX, double posY)
{
   RotateCamera(g_MainCamera, posX, posY);
}

void InputHandle()
{
   if (g_KeyStates[GLFW_KEY_W])
      MoveCamera(g_MainCamera, CameraMoveForward);
   if(g_KeyStates[GLFW_KEY_S])
      MoveCamera(g_MainCamera, CameraMoveBackward);
   if(g_KeyStates[GLFW_KEY_D])
      MoveCamera(g_MainCamera, CameraMoveRight);
   if (g_KeyStates[GLFW_KEY_A])
      MoveCamera(g_MainCamera, CameraMoveLeft);
   if (g_KeyStates[GLFW_KEY_E])
      MoveCamera(g_MainCamera, CameraMoveUp);
   if (g_KeyStates[GLFW_KEY_Q])
      MoveCamera(g_MainCamera, CameraMoveDown);
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

   g_MainCamera = CreateCamera((Vector3) { 0.0f, 0.0f, 10.0f }, PI / 4, 1.7f, 0.1f);

   Scene* scene = CreateScene(g_MainCamera);

   MeshData cubeMeshData = LoadMesh("res/meshes/cube.obj");
   AddRenderObject(scene, CreateRenderObject(CreateMesh(cubeMeshData.Positions, cubeMeshData.Normals, cubeMeshData.FacesCount), NULL, CreateTranslateMatrix((Vector3) { 0.0f, 0.0f, 0.0f })));

   InitializeForwardRender();

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      InputHandle();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      UpdateForwardRender(scene);

      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}
