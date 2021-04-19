#include "math/matrices/matrix4x4.h"

#include "asset-manager/asset-manager.h"

#include "math/math_utils.h"
#include "utils/timer.h"

#include "jobs/job-system.h"

#include "events/subject.h"

#include "utils/read-from-file.h"

#include "entry-point/entry-point.h"

#include "graphics/api/devices/gl-device.h"

#include "debug/globals.h"

#include "graphics/mesh-render.h"

#include "graphics/camera/camera.h"

#include "graphics/scene.h"

#include "math/math.h"

int main()
{
   app::CreateEngineApp();

   graphics::Camera MainCamera;
   MainCamera = graphics::Camera(mm::vec3(0.0f, 0.0f, 10.0f), math::Pi / 4, 1.7f, 5.0f);

   assets::AssetManager AssetManager;

   uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(&MainCamera);

   g_InputManager->AddAxisMapping("MoveForward", { { input::InputEvent::W, 1.0f },
                                            { input::InputEvent::S, -1.0f } });
   g_InputManager->AddAxisMapping("MoveRight", { { input::InputEvent::D, 1.0f },
                                              { input::InputEvent::A, -1.0f} });
   g_InputManager->AddAxisMapping("MoveUp", { { input::InputEvent::E, 1.0f },
                                           { input::InputEvent::Q, -1.0f } });

   g_InputManager->BindAxis("MoveForward", [](const float value, const uintptr_t args)
      {
         graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
         camera->Move(graphics::CameraMoveType::MoveForward, value, app::g_DeltaTime);
      }, callbackArgs);

   g_InputManager->BindAxis("MoveRight", [](const float value, const uintptr_t args)
      {
         graphics::Camera* camera = reinterpret_cast<graphics::Camera*>(args);
         camera->Move(graphics::CameraMoveType::MoveRight, value, app::g_DeltaTime);
      }, callbackArgs);
   g_InputManager->BindAxis("MoveUp", [](const float value, const uintptr_t args)
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
   g_Window->GetCanvas()->AddCursorCallback(cameraCallback);

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

   auto pistolDifData = AssetManager.GetData<assets::PixelsData>(pistolDifPath);
   auto pistolNormData = AssetManager.GetData<assets::PixelsData>(pistolNormPath);

   auto brickDifData = AssetManager.GetData<assets::PixelsData>(brickDifPath);
   auto brickNormData = AssetManager.GetData<assets::PixelsData>(brickNormPath);

   graphics::TextureParams params;
   params.MagFilter = graphics::TextureFilter::Nearest;
   params.MinFilter = graphics::TextureFilter::Nearest;
   params.WrapS = graphics::TextureWrap::ClampToEdge;
   params.WrapT = graphics::TextureWrap::ClampToEdge;

   auto& pistolDiffuse = g_GraphicsDevice->CreateTexture2D();
   pistolDiffuse->InitData(pistolDifData->Width, pistolDifData->Height,
                           graphics::InternalFormat::RGB8, graphics::Format::RGB,
                           graphics::Type::Ubyte, params);
   pistolDiffuse->UpdateData(pistolDifData->Width, pistolDifData->Height, pistolDifData->Pixels);

   auto& pistolNorm = g_GraphicsDevice->CreateTexture2D();
   pistolNorm->InitData(pistolNormData->Width, pistolNormData->Height,
                           graphics::InternalFormat::RGB8, graphics::Format::RGB,
                           graphics::Type::Ubyte, params);
   pistolNorm->UpdateData(pistolNormData->Width, pistolNormData->Height, pistolNormData->Pixels);

   auto& cubeDiffuse = g_GraphicsDevice->CreateTexture2D();
   cubeDiffuse->InitData(brickDifData->Width, brickDifData->Height,
      graphics::InternalFormat::RGB8, graphics::Format::RGB,
      graphics::Type::Ubyte, params);
   cubeDiffuse->UpdateData(brickDifData->Width, brickDifData->Height, brickDifData->Pixels);

   auto& cubeNorm = g_GraphicsDevice->CreateTexture2D();
   cubeNorm->InitData(brickNormData->Width, brickNormData->Height,
      graphics::InternalFormat::RGB8, graphics::Format::RGB,
      graphics::Type::Ubyte, params);
   cubeNorm->UpdateData(brickNormData->Width, brickNormData->Height, brickNormData->Pixels);
   
   graphics::PhongMaterial pistolM;
   pistolM.Diffuse = { 1.0f, 0.2f, 0.5f, 1.0f };
   pistolM.Specular = mm::vec3(0.8f);
   pistolM.Glossiness = 8.0f;
   pistolM.DiffuseTexture = pistolDiffuse;
   pistolM.NormalTexture = pistolNorm;

   graphics::PhongMaterial cubeM;
   cubeM.Diffuse = { 1.0f, 0.2f, 0.5f, 1.0f };
   cubeM.Specular = mm::vec3(0.8f);
   cubeM.Glossiness = 8.0f;
   cubeM.DiffuseTexture = cubeDiffuse;
   cubeM.NormalTexture = pistolNorm;

   graphics::Mesh pistolMesh;
   pistolMesh.Vertices = *pistolData;
   pistolMesh.Material = &pistolM;
   pistolMesh.Scale = { 3.0f, 3.0f, 3.0f };

   graphics::Mesh cubeMesh;
   cubeMesh.Vertices = *cubeData;
   cubeMesh.Material = &cubeM;
   cubeMesh.Scale = { 5.0f, 0.5f, 5.0f };
   cubeMesh.Translate = { 0.0f, -3.0f, -5.0f };

   scene::Scene scene;
   scene::Register(scene, &pistolMesh);
   scene::Register(scene, &cubeMesh);
   scene::Register(scene, new graphics::PointLight({ 0.0f, 3.0f, -5.0f }, { 1.0f, 1.0f, 1.0f }, 10.0f, 3.0f));
   scene::Register(scene, &MainCamera);

   app::RunEngineApp([&]()
      {
         scene::UpdateAndRender(scene);
      });

   return 0;
}