#include "scene/scene.h"

#include "math/matrices/matrix4x4.h"
#include "scene/renders/forward-render.h"
#include "graphics/shaders/shader-pipeline.h"

#include "asset-manager/obj-loader.h"
#include "asset-manager/image-loader.h"
#include "asset-manager/text-loader.h"
#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"
#include "utils/timer.h"

#include "gui.h"

#include "jobs/job-system.h"

#include "application/window/window-wrapper.h"

#include "input/input-manager.h"

#include "events/subject.h"

#include "application/application.h"

#include "utils/read-from-file.h"

#include "debug/graphics/debug-draw-manager.h"

class MainApp : public app::BaseApplication
{
public:
   graphics::Camera MainCamera;
   assets::AssetManager AssetManager;
   core::Scene Scene;

   void OnStartup() override
   {
      uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(this);

      InputManager->AddAxisMapping("MoveForward", { { input::InputEvent::W, 1.0f },
                                                   { input::InputEvent::S, -1.0f } });
      InputManager->AddAxisMapping("MoveRight", { { input::InputEvent::D, 1.0f },
                                                 { input::InputEvent::A, -1.0f} });
      InputManager->AddAxisMapping("MoveUp", { { input::InputEvent::E, 1.0f },
                                              { input::InputEvent::Q, -1.0f } });

      InputManager->BindAxis("MoveForward", [](const float value, const uintptr_t args)
         {
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Move(graphics::CameraMoveType::MoveForward, value, app->DeltaTime);
         }, callbackArgs);

      InputManager->BindAxis("MoveRight", [](const float value, const uintptr_t args)
         {
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Move(graphics::CameraMoveType::MoveRight, value, app->DeltaTime);
         }, callbackArgs);
      InputManager->BindAxis("MoveUp", [](const float value, const uintptr_t args)
         {
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Move(graphics::CameraMoveType::MoveUp, value, app->DeltaTime);
         }, callbackArgs);

      InputManager->GetInputWrapper()->MouseCursorPosEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
         {
            event::MouseCursorPosEvent mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
            MainApp* app = reinterpret_cast<MainApp*>(args);
            app->MainCamera.Rotate(mouseE.PosX, mouseE.PosY, app->DeltaTime);
         }, callbackArgs);

      assets::AssetRef pistolAssetRef = AssetManager.RequireAsssetRef("res/meshes/pistol/pistol.obj");
      assets::AssetRef cubeAssetRef = AssetManager.RequireAsssetRef("res/meshes/cube.obj");

      assets::AssetRef pistolTextureRef = AssetManager.RequireAsssetRef("res/meshes/pistol/textures/handgun_C.jpg");

      //for(size_t i = 0; i < 15; ++i)
      //  AssetManager.RequireAsssetId("res/meshes/pistol/pistol.obj");

      {
         utils::Timer assetTimer(true);
         AssetManager.Load();
         LOG_WARNING("Assets loading time: %f ms", assetTimer.GetElaspedTime());
      }

      MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), math::Pi / 4, 1.7f, 5.0f);

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

      Scene.SetCamera(&MainCamera);

      size_t floorMaterial = Scene.AddMaterial(floorM);
      size_t pistolMaterial = Scene.AddMaterial(pistolM);

      Scene.AddRenderObject({ graphics::Mesh(pistolAssetRef), CreateTranslateMatrix(math::Vector3(0.0f, 0.0f, 0.0f)), pistolMaterial });

      math::Matrix4 cubeTransform = CreateTranslateMatrix(math::Vector3(0.0f, -2.0f, 0.0f));
      cubeTransform = cubeTransform * CreateScaleMatrix(math::Vector3(15.0f, 1.0f, 15.0f));
      Scene.AddRenderObject({ graphics::Mesh(cubeAssetRef), cubeTransform, floorMaterial });

      Scene.AddLight(graphics::PointLight(math::Vector3(1.2f, 1.0f, 2.0f), math::Vector3(1.0f, 1.0f, 1.0f), 0.09f, 1.0f, 0.032f));

      /* for (int i = 0; i < 1; ++i)
          Scene.AddLight(graphics::Spotlight(math::Vector3(0, 10.0f, 0), math::Vector3(0.0f, -1.0f, 0.0f), math::Vector3(1.0f, 1.0f, 1.0f), math::Pi / 12, math::Pi / 15));*/

      graphics::ForwardRender::Initialize();
   }

   void OnTick() override
   {
      graphics::ForwardRender::Update(Scene, DeltaTime);
   }
};

app::BaseApplication* app::CreateApplication()
{
   return new MainApp();
}
