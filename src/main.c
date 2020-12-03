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

   MeshData cubeMeshData = LoadMesh("res/meshes/cube.obj");

   Matrix4 m1 = CreateIdentityMatrix4();
   Matrix4 m2 = CreateTranslateMatrix((Vector3){ 5.0f, 3.0f, 7.0f });

   Vector4 pos = (Vector4){10.0f, 6.0f, 10.0f, 1.0f};

   pos = multiply_vmatrix4(pos, m1);

   Scene* scene = CreateScene();
   AddRenderObject(scene, CreateRenderObject(CreateMesh(cubeMeshData.Positions, cubeMeshData.PositionsCount), NULL, CreateTranslateMatrix((Vector3) { 0.0f, 0.0f, -5.0f })));

   Camera* camera = CreateCamera((Vector3) { 0.0f, 0.0f, 0.0f }, PI / 4, 1.7f, 10.0f);

   InitializeForwardRender();

   while (!glfwWindowShouldClose(window))
   {
      glfwPollEvents();
      glClear(GL_COLOR_BUFFER_BIT);

      UpdateForwardRender(scene, camera);

      glfwSwapBuffers(window);
   }

   glfwTerminate();
   return 0;
}
