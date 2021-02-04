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
#include "utils/timer.h"
#include "utils/sync/spin-lock.h"

#include <cstring>
#include <Windows.h>

#include "gui.h"
#include "bgl/shader.h"

#include "debug/log/log.h"
#include "bgl/debug/callback.h"

#include "jobs/job-system.h"

#include "bgl/window/window-wrapper.h"

char g_KeyStates[1024];
graphics::Camera g_MainCamera;

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
   g_MainCamera.Rotate(posX, posY, g_DeltaTime);
}

void InputHandle()
{
   if (g_KeyStates[GLFW_KEY_W])
      g_MainCamera.Move(graphics::CameraMoveType::MoveForward, g_DeltaTime);
   if(g_KeyStates[GLFW_KEY_S])
      g_MainCamera.Move(graphics::CameraMoveType::MoveBackward, g_DeltaTime);
   if(g_KeyStates[GLFW_KEY_D])
      g_MainCamera.Move(graphics::CameraMoveType::MoveRight, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_A])
      g_MainCamera.Move(graphics::CameraMoveType::MoveLeft, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_E])
      g_MainCamera.Move(graphics::CameraMoveType::MoveUp, g_DeltaTime);
   if (g_KeyStates[GLFW_KEY_Q])
      g_MainCamera.Move(graphics::CameraMoveType::MoveDown, g_DeltaTime);
}

int main()
{
   bgl::WindowGL window;
   window.Instantiate();

   glfwSetKeyCallback(window.NativeHandle.get(), KeyCallback);
   glfwSetCursorPosCallback(window.NativeHandle.get(), CursorCallback);

   core::JobSystem::Setup();

   assets::AssetManager assetManager;

   assets::AssetRef pistolAssetRef = assetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
   assets::AssetRef cubeAssetRef = assetManager.RequireAsssetRef("res/meshes/cube.obj");

   assets::AssetRef pistolTextureRef = assetManager.RequireAsssetRef("res/meshes/pistol/textures/handgun_C.jpg");
  
   //for(size_t i = 0; i < 15; ++i)
   //  assetManager.RequireAsssetId("res/meshes/pistol/pistol.obj");

   {
      utils::Timer assetTimer(true);
      assetManager.Load();
      LOG_WARNING("Assets loading time: %f ms", assetTimer.GetElaspedTime());
   }

   g_MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), PI / 4, 1.7f, 5.0f);

   graphics::Material floorM;
   floorM.Color = math::Vector3(0.0f, 0.2f, 0.2f);
   floorM.Specular = math::Vector3(0.5f, 0.5f, 0.5f);
   floorM.ShineExponent = 10.0f;
   floorM.Emissive = math::Vector3(0.0f);

   graphics::Material pistolM;
   pistolM.Color = math::Vector3(0.7f, 0.2f, 0.2f);
   pistolM.DiffuseTexture = pistolTextureRef;
   pistolM.Specular = math::Vector3(1.0f);
   pistolM.ShineExponent = 10.0f;
   pistolM.Emissive = math::Vector3(0.0f);

   core::Scene scene;
   scene.SetCamera(&g_MainCamera);

   size_t floorMaterial  = scene.AddMaterial(floorM);
   size_t pistolMaterial = scene.AddMaterial(pistolM);

   scene.AddRenderObject({ graphics::Mesh(pistolAssetRef), CreateTranslateMatrix(math::Vector3(0.0f, 0.0f, 0.0f)), pistolMaterial });

   math::Matrix4 cubeTransform = CreateTranslateMatrix(math::Vector3(0.0f, -2.0f, 0.0f ));
   cubeTransform = cubeTransform * CreateScaleMatrix(math::Vector3(5.0f, 1.0f, 5.0f));
   scene.AddRenderObject({ graphics::Mesh(cubeAssetRef), cubeTransform, floorMaterial });
   
   scene.AddLight(graphics::PointLight(math::Vector3(1.2f, 1.0f, 2.0f), math::Vector3(1.0f, 1.0f, 1.0f), 0.09f, 1.0f, 0.032f));

   for(int i = 0; i < 1; ++i)
      scene.AddLight(graphics::Spotlight(math::Vector3(0, 10.0f, 0), math::Vector3(0.0f, -1.0f, 0.0f), math::Vector3(1.0f, 1.0f, 1.0f), PI / 12, PI / 15));

   graphics::ForwardRender::Initialize();

   LARGE_INTEGER freq;
   QueryPerformanceFrequency(&freq);

   utils::Timer deltaTimer;

   while (!window.ShouldClose())
   {
      deltaTimer.Reset();

      window.BeginFrame();

      InputHandle();
      
      graphics::ForwardRender::Update(scene, g_DeltaTime);

      window.EndFrame();

      g_DeltaTime = 1.0f / deltaTimer.GetElaspedTime();
   }

   glfwTerminate();
   return 0;
}