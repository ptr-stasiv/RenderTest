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

   glfwSetKeyCallback(window, KeyCallback);
   glfwSetCursorPosCallback(window, CursorCallback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   MeshData cubeMeshData = LoadMesh("res/meshes/cube.obj");

   Matrix4 m1 = CreateIdentityMatrix4();
   Matrix4 m2 = CreateTranslateMatrix((Vector3){ 5.0f, 3.0f, 7.0f });

   Vector4 pos = (Vector4){10.0f, 6.0f, 10.0f, 1.0f};

   pos = multiply_vmatrix4(pos, m1);

   Scene* scene = CreateScene();
   AddRenderObject(scene, CreateRenderObject(CreateMesh(cubeMeshData.Positions, cubeMeshData.PositionsCount), NULL, CreateTranslateMatrix((Vector3) { 0.0f, 0.0f, -5.0f })));

   g_MainCamera = CreateCamera((Vector3) { 0.0f, 0.0f, 10.0f }, PI / 4, 1.7f, 1.0f);

   InitializeForwardRender();

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      InputHandle();

      glClear(GL_COLOR_BUFFER_BIT);

      UpdateForwardRender(scene, g_MainCamera);

      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}
