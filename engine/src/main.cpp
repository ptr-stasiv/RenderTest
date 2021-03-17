#include "math/matrices/matrix4x4.h"

#include "asset-manager/obj-loader.h"
#include "asset-manager/image-loader.h"
#include "asset-manager/text-loader.h"
#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"
#include "utils/timer.h"

#include "gui.h"

#include "jobs/job-system.h"

#include "events/subject.h"

#include "utils/read-from-file.h"

#include "debug/graphics/debug-draw-manager.h"

#include "entry-point/entry-point.h"

#include "graphics/renders/forward-render.h"

#include "graphics/api/devices/gl-device.h"

int main()
{
   app::CreateEngineApp();


   //graphics::Camera MainCamera;
   //assets::AssetManager AssetManager;

   //uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(&MainCamera);

   //app::g_InputManager->AddAxisMapping("MoveForward", { { input::InputEvent::W, 1.0f },
   //                                         { input::InputEvent::S, -1.0f } });
   //app::g_InputManager->AddAxisMapping("MoveRight", { { input::InputEvent::D, 1.0f },
   //                                           { input::InputEvent::A, -1.0f} });
   //app::g_InputManager->AddAxisMapping("MoveUp", { { input::InputEvent::E, 1.0f },
   //                                        { input::InputEvent::Q, -1.0f } });

   //app::g_InputManager->BindAxis("MoveForward", [](const float value, const uintptr_t args)
   //   {
   //      graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
   //      camera->Move(graphics::CameraMoveType::MoveForward, value, app::g_DeltaTime);
   //   }, callbackArgs);

   //app::g_InputManager->BindAxis("MoveRight", [](const float value, const uintptr_t args)
   //   {
   //      graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
   //      camera->Move(graphics::CameraMoveType::MoveRight, value, app::g_DeltaTime);
   //   }, callbackArgs);
   //app::g_InputManager->BindAxis("MoveUp", [](const float value, const uintptr_t args)
   //   {
   //      graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
   //      camera->Move(graphics::CameraMoveType::MoveUp, value, app::g_DeltaTime);
   //   }, callbackArgs);

   ////This temporal solution for smooth camera move
   ////Its will have reworked soon
   //event::Subject cameraSubj;
   //cameraSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
   //   {
   //      event::MouseCursorPosEvent mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
   //      graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
   //      camera->Rotate(mouseE.PosX, mouseE.PosY, app::g_DeltaTime);
   //   }, callbackArgs);
   //input::native::AddCursorCallback(cameraSubj);

   //assets::AssetRef pistolAssetRef = AssetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
   //assets::AssetRef cubeAssetRef = AssetManager.RequireAsssetRef("res/meshes/cube.obj");

   //assets::AssetRef pistolTextureRef = AssetManager.RequireAsssetRef("res/meshes/pistol/textures/handgun_C.jpg");

   //{
   //   utils::Timer assetTimer(true);
   //   AssetManager.Load();
   //   LOG_WARNING("Assets loading time: %f ms", assetTimer.GetElapsedTime());
   //}

   //MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), math::Pi / 4, 1.7f, 5.0f);

   //graphics::Mesh pistolMesh(pistolAssetRef.GetData<assets::MeshAssetData>()->Positions, pistolAssetRef.GetData<assets::MeshAssetData>()->Positions, pistolAssetRef.GetData<assets::MeshAssetData>()->UVs);
   //graphics::Mesh cubeMesh(cubeAssetRef.GetData<assets::MeshAssetData>()->Positions, cubeAssetRef.GetData<assets::MeshAssetData>()->Positions, cubeAssetRef.GetData<assets::MeshAssetData>()->UVs);

   //graphics::Material pistolM;      
   //pistolM.DiffuseColor = math::Vector3(0.5f, 0.2f, 0.5f);
   //pistolM.SpecularColor = math::Vector3(0.8f);
   //pistolM.Glossiness = 16.0f;

   //graphics::Material cubeM;
   //cubeM.DiffuseColor = math::Vector3(0.3f, 0.3f, 0.3f);
   //cubeM.SpecularColor = math::Vector3(1.0f);
   //cubeM.Glossiness = 8.0f;

   //graphics::ForwardRender fr;

   //fr.AddRenderer({ pistolMesh, pistolM, math::CreateIdentityMatrix4() });
   //fr.AddRenderer({ cubeMesh, pistolM, math::CreateTranslateMatrix({ 0.0f, -3.0f, 0.0f }) 
   //                                * math::CreateScaleMatrix({ 5.0f, 0.5f, 5.0f }) });

   app::RunEngineApp([&]()
      {
         //fr.Render(MainCamera);
      });

   return 0;
}