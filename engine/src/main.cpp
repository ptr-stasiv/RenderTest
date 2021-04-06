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

#include "graphics/mesh-render.h"

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

   auto pistolDifData = AssetManager.GetData<assets::PixelsData>(pistolDifPath);
   auto pistolNormData = AssetManager.GetData<assets::PixelsData>(pistolNormPath);

   auto brickDifData = AssetManager.GetData<assets::PixelsData>(brickDifPath);
   auto brickNormData = AssetManager.GetData<assets::PixelsData>(brickNormPath);

   graphics::TextureParams universalTextureParams;
   universalTextureParams.MagFilter = graphics::TextureFilter::Linear;
   universalTextureParams.MinFilter = graphics::TextureFilter::Nearest;
   universalTextureParams.WrapS = graphics::TextureWrap::ClampToEdge;
   universalTextureParams.WrapT = graphics::TextureWrap::ClampToEdge;

   auto& pistolDiffuse = app::g_GraphicsDevice->CreateTexture2D();
   pistolDiffuse->InitData(pistolDifData->Width, pistolDifData->Height,
                           graphics::InternalFormat::RGB8, graphics::Format::RGB,
                           graphics::Type::Ubyte, universalTextureParams);
   pistolDiffuse->UpdateData(pistolDifData->Width, pistolDifData->Height, pistolDifData->Pixels);

   auto& pistolNorm = app::g_GraphicsDevice->CreateTexture2D();
   pistolNorm->InitData(pistolNormData->Width, pistolNormData->Height,
                        graphics::InternalFormat::RGB8, graphics::Format::RGB,
                        graphics::Type::Ubyte, universalTextureParams);
   pistolNorm->UpdateData(pistolNormData->Width, pistolNormData->Height, pistolNormData->Pixels);

   auto& brickDiffuse = app::g_GraphicsDevice->CreateTexture2D();
   brickDiffuse->InitData(brickDifData->Width, brickDifData->Height,
                          graphics::InternalFormat::RGB8, graphics::Format::RGB,
                          graphics::Type::Ubyte, universalTextureParams);
   brickDiffuse->UpdateData(brickDifData->Width, brickDifData->Height, brickDifData->Pixels);

   auto& brickNorm = app::g_GraphicsDevice->CreateTexture2D();
   brickNorm->InitData(brickNormData->Width, brickNormData->Height,
                          graphics::InternalFormat::RGB8, graphics::Format::RGB,
                          graphics::Type::Ubyte, universalTextureParams);
   brickNorm->UpdateData(brickNormData->Width, brickNormData->Height, brickNormData->Pixels);

   graphics::PhongMaterial pistolM(app::g_GraphicsDevice);
   pistolM.Diffuse = { 1.0f, 0.2f, 0.5f, 1.0f };
   pistolM.Specular = math::Vector3(0.8f);
   pistolM.Glossiness = 8.0f;
   pistolM.DiffuseTexture = pistolDiffuse;
   pistolM.NormalTexture = pistolNorm;

   graphics::PhongMaterial cubeM(app::g_GraphicsDevice);
   cubeM.Diffuse = { 0.3f, 0.3f, 0.3f, 1.0f };
   cubeM.Specular = math::Vector3(1.0f);
   cubeM.Glossiness = 8.0f;
   cubeM.DiffuseTexture = brickDiffuse;
   cubeM.NormalTexture = brickNorm;

   MainCamera = graphics::Camera(math::Vector3(0.0f, 0.0f, 10.0f), math::Pi / 4, 1.7f, 5.0f);

   graphics::Mesh pistolMesh;
   pistolMesh.Vertices = *pistolData;
   pistolMesh.Material = std::shared_ptr<graphics::PhongMaterial>(&pistolM);

   graphics::Mesh cubeMesh;
   cubeMesh.Vertices = *cubeData;
   cubeMesh.Scale = { 5.0f, 0.5f, 5.0f };
   cubeMesh.Translate = { 0.0f, -3.0f, 0.0f };
   cubeMesh.Material = std::shared_ptr<graphics::PhongMaterial>(&cubeM);


   app::g_Renderer->AddRenderer(pistolMesh);
   app::g_Renderer->AddRenderer(cubeMesh);

   app::g_Renderer->AddLight(graphics::PointLight({ 0.0f, 3.0f, -5.0f }, { 1.0f }, 10.0f, 3.0f));
   //app::g_Renderer->AddLight(graphics::Spotlight({ 0.0f, 3.0f, 10.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f }, math::Pi / 2.0f, math::Pi / 3.0f));

   app::RunEngineApp([&]()
      {
         app::g_Renderer->Render(MainCamera);
      });

   return 0;
}