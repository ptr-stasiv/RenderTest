#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "scene/scene.h"

#include "math/matrices/matrix4x4.h"
#include "scene/renders/forward-render.h"

#include "asset-manager/obj-loader.h"
#include "asset-manager/image-loader.h"
#include "asset-manager/text-loader.h"
#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"

#include <string.h>
#include <Windows.h>

#include "gui.h"
#include "bgl/shader.h"

#include "debug/log/log.h"
#include "bgl/debug/callback.h"

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

   const GLubyte* vendorInfo   = glGetString(GL_VENDOR);
   const GLubyte* rendererInfo = glGetString(GL_RENDERER);
   const GLubyte* versionInfo  = glGetString(GL_VERSION);
   const GLubyte* glslInfo     = glGetString(GL_SHADING_LANGUAGE_VERSION);

   WD_LOG_MESSAGE("%s %s", vendorInfo, rendererInfo);
   WD_LOG_MESSAGE("Opengl %s", versionInfo);
   WD_LOG_MESSAGE("GLSL %s\n", glslInfo);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendEquation(GL_FUNC_ADD);

   glEnable(GL_DEPTH_TEST);

   glfwSetKeyCallback(window, KeyCallback);
   glfwSetCursorPosCallback(window, CursorCallback);

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

   glEnable(GL_DEBUG_OUTPUT);
   glDebugMessageCallback(MessageCallbackOGL, 0);

   assets::AssetManager assetManager;

   assets::AssetRef pistolAssetRef = assetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
   assets::AssetRef cubeAssetRef = assetManager.RequireAsssetRef("res/meshes/cube.obj");

   assets::AssetRef pistolTextureRef = assetManager.RequireAsssetRef("res/meshes/pistol/textures/handgun_C.jpg");
  
   assetManager.Load();

   g_MainCamera = CreateCamera(vec3(0.0f, 0.0f, 10.0f), PI / 4, 1.7f, 5.0f);
   

   graphics::Material floorM;
   floorM.Color = vec3(0.0f, 0.2f, 0.2f);
   floorM.Specular = vec3(0.5f, 0.5f, 0.5f);
   floorM.ShineExponent = 10.0f;
   floorM.Emissive = svec3(0.0f);

   graphics::Material pistolM;
   pistolM.Color = vec3(0.7f, 0.2f, 0.2f);
   pistolM.DiffuseTexture = pistolTextureRef;
   pistolM.Specular = svec3(1.0f);
   pistolM.ShineExponent = 10.0f;
   pistolM.Emissive = svec3(0.0f);

   core::Scene scene(*g_MainCamera);

   size_t floorMaterial  = scene.AddMaterial(floorM);
   size_t pistolMaterial = scene.AddMaterial(pistolM);

   scene.AddRenderObject(CreateRenderObject(CreateMesh(pistolAssetRef), CreateTranslateMatrix(vec3(0.0f, 0.0f, 0.0f)), pistolMaterial));

   Matrix4 cubeTransform = CreateTranslateMatrix(vec3(0.0f, -2.0f, 0.0f ));
   cubeTransform = multiply_matrix(cubeTransform, CreateScaleMatrix(vec3(5.0f, 1.0f, 5.0f)));
   scene.AddRenderObject(CreateRenderObject(CreateMesh(cubeAssetRef), cubeTransform, floorMaterial));
   
   scene.AddLight(CreatePointLight(vec3(1.2f, 1.0f, 2.0f), vec3(1.0f, 1.0f, 1.0f), 0.09f, 1.0f, 0.032f));

   for(int i = 0; i < 1; ++i)
      scene.AddLight(CreateSpotlight(vec3(0.0f, 10.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), PI / 12, PI / 15));

   graphics::ForwardRender::Initialize();

   LARGE_INTEGER freq;
   QueryPerformanceFrequency(&freq);

   while (!glfwWindowShouldClose(window))
   {
      LARGE_INTEGER startFrameTicks;
      QueryPerformanceCounter(&startFrameTicks);


      glfwPollEvents();
      InputHandle();

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      graphics::ForwardRender::Update(scene, g_DeltaTime);

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
