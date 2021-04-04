#include "math/matrices/matrix4x4.h"

#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"
#include "utils/timer.h"

#include "jobs/job-system.h"

#include "events/subject.h"

#include "utils/read-from-file.h"

#include "entry-point/entry-point.h"

#include "graphics/renders/forward-render.h"

#include "graphics/api/devices/gl-device.h"

#include "debug/globals.h"

int main()
{
   app::CreateEngineApp();

   graphics::Camera MainCamera;

   assets::AssetManager AssetManager;

   uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(&MainCamera);

   app::g_InputManager->AddAxisMapping("MoveForward", { { input::InputEvent::W, 1.0f },
                                            { input::InputEvent::S, -1.0f } });
   app::g_InputManager->AddAxisMapping("MoveRight", { { input::InputEvent::D, 1.0f },
                                              { input::InputEvent::A, -1.0f} });
   app::g_InputManager->AddAxisMapping("MoveUp", { { input::InputEvent::E, 1.0f },
                                           { input::InputEvent::Q, -1.0f } });

   app::g_InputManager->BindAxis("MoveForward", [](const float value, const uintptr_t args)
      {
         graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
         camera->Move(graphics::CameraMoveType::MoveForward, value, app::g_DeltaTime);
      }, callbackArgs);

   app::g_InputManager->BindAxis("MoveRight", [](const float value, const uintptr_t args)
      {
         graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
         camera->Move(graphics::CameraMoveType::MoveRight, value, app::g_DeltaTime);
      }, callbackArgs);
   app::g_InputManager->BindAxis("MoveUp", [](const float value, const uintptr_t args)
      {
         graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
         camera->Move(graphics::CameraMoveType::MoveUp, value, app::g_DeltaTime);
      }, callbackArgs);

   //This temporal solution for smooth camera move
   //Its will have reworked soon
   event::Callback cameraCallback = { [](event::BaseEvent& e, uintptr_t args)
      {
         event::MouseCursorPosEvent mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
         graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
         camera->Rotate(mouseE.PosX, mouseE.PosY, app::g_DeltaTime);
      }, callbackArgs };
   app::g_Window->GetCanvas()->AddCursorCallback(cameraCallback);


   constexpr auto pistolPath = "res/meshes/pistol/pistol.obj";
   constexpr auto cubePath = "res/meshes/cube.obj";
   constexpr auto pistolDifPath = "res/meshes/pistol/textures/handgun_C.jpg";
   constexpr auto pistolNormPath = "res/meshes/pistol/textures/handgun_N.jpg";
   constexpr auto brickDifPath = "res/textures/brickwall.jpg";
   constexpr auto brickNormPath = "res/textures/brickwall_normal.jpg";

   AssetManager.ToLoad(pistolPath);
   AssetManager.ToLoad(cubePath);

   AssetManager.ToLoad(pistolDifPath);
   AssetManager.ToLoad(pistolNormPath);
   AssetManager.ToLoad(brickDifPath);
   AssetManager.ToLoad(brickNormPath);

   {
      utils::Timer assetTimer(true);
      AssetManager.Load();
      LOG_WARNING("Assets loading time: %f ms", assetTimer.GetElapsedTime());
   }

   auto pistolData = AssetManager.GetData<assets::TrigVertices>(pistolPath);
   auto cubeData = AssetManager.GetData<assets::TrigVertices>(cubePath);

   MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), math::Pi / 4, 1.7f, 5.0f);

   graphics::Mesh pistolMesh(pistolData->Positions, 
                             pistolData->Normals, 
                             pistolData->UVs,
                             pistolData->Tangents,
                             pistolData->Bitangents);

   graphics::Mesh cubeMesh(cubeData->Positions,
                           cubeData->Normals, 
                           cubeData->UVs,
                           cubeData->Tangents,
                           cubeData->Bitangents);
   
   graphics::Material pistolM;
   pistolM.DiffuseColor = { 1.0f, 0.2f, 0.5f, 1.0f };
   pistolM.SpecularColor = math::Vector3(0.8f);
   pistolM.Glossiness = 8.0f;
   pistolM.DiffuseTexture = *AssetManager.GetData<assets::PixelsData>(pistolDifPath);
   pistolM.NormalTexture = *AssetManager.GetData<assets::PixelsData>(pistolNormPath);

   graphics::Material cubeM;
   cubeM.DiffuseColor = { 0.3f, 0.3f, 0.3f, 1.0f };
   cubeM.SpecularColor = math::Vector3(1.0f);
   cubeM.Glossiness = 8.0f;
   cubeM.DiffuseTexture = *AssetManager.GetData<assets::PixelsData>(brickDifPath);
   cubeM.NormalTexture = *AssetManager.GetData<assets::PixelsData>(brickNormPath);


   app::g_Renderer->AddRenderer({ pistolMesh, pistolM, math::CreateIdentityMatrix4() });
   app::g_Renderer->AddRenderer({ cubeMesh, cubeM, math::CreateTranslateMatrix({ 0.0f, -3.0f, 0.0f })
                                   * math::CreateScaleMatrix({ 5.0f, 0.5f, 5.0f }) });

   app::g_Renderer->AddLight(graphics::PointLight({ 0.0f, 3.0f, -5.0f }, { 1.0f }, 10.0f, 3.0f));
   //app::g_Renderer->AddLight(graphics::Spotlight({ 0.0f, 3.0f, 10.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f }, math::Pi / 2.0f, math::Pi / 3.0f));

   app::RunEngineApp([&]()
      {
         app::g_Renderer->Render(MainCamera);
      });

   return 0;
}