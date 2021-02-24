#include "gui.h"

#include <cstdio>
#include <string>
#include <bitset>

#include "Ultralight/Ultralight.h"
#include "AppCore/Platform.h"

#include "key-map.h"
#include "platforms/declarations/window/platform-input-map.h"
#include "platforms/declarations/process/process.h"

namespace gui
{
   constexpr auto ServerApplicationPath = "D:/Own/RenderTest/gui/extern/WebGui/server/server.exe";
   constexpr auto ServerWorkingDir      = "D:/Own/RenderTest/gui/extern/WebGui/server";

   struct UlInfoPimpl
   {
      ultralight::RefPtr<ultralight::Renderer> Renderer;
      ultralight::RefPtr<ultralight::View> View;

      std::unique_ptr<platform::ProcessHandle> ServerHandle;
   };

   GuiController::GuiController()
      : UlInfo(new UlInfoPimpl) {}

   GuiController::~GuiController() = default;

   void GuiController::OnMouseButton(const uint32_t button, const uint32_t state)
   {
      ultralight::MouseEvent e;
      e.type = !state ? ultralight::MouseEvent::Type::kType_MouseUp : ultralight::MouseEvent::Type::kType_MouseDown;
      e.button = static_cast<ultralight::MouseEvent::Button>(input::UltralightInputMap[platform::input::PlatformInputMap[button]]);

      UlInfo->View->FireMouseEvent(e);
   }

   void GuiController::OnMouseMove(const float x, const float y)
   {
      ultralight::MouseEvent e;
      e.type = ultralight::MouseEvent::Type::kType_MouseMoved;
      e.x    = round(x);
      e.y    = round(y);
      e.button = ultralight::MouseEvent::Button::kButton_None;

      UlInfo->View->FireMouseEvent(e);
   }

   void GuiController::OnMouseScroll(const float value)
   {
      ultralight::ScrollEvent e;
      e.type = ultralight::ScrollEvent::Type::kType_ScrollByPixel;
      e.delta_x = 0;
      e.delta_y = round(value) * 30.0f;

      UlInfo->View->FireScrollEvent(e);
   }

   void GuiController::Setup(const uint32_t resX, const uint32_t resY)
   {
      UlInfo->ServerHandle = std::make_unique<platform::ProcessHandle>(ServerApplicationPath, ServerWorkingDir);

      ultralight::Config config;

      config.resource_path = "./resources/";

      config.use_gpu_renderer = false;

      config.device_scale = 1.0f;

      ultralight::Platform::instance().set_config(config);


      ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());

      ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));

      ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));


      UlInfo->Renderer = ultralight::Renderer::Create();

      UlInfo->View = UlInfo->Renderer->CreateView(resX, resY, true, nullptr);

      UlInfo->View->LoadURL("http://localhost:3333/");

      UlInfo->View->Focus();
   }

   void GuiController::GetRenderingInfo(uint32_t& resX, uint32_t& resY, void*& pixels)
   {
      UlInfo->Renderer->Update();
      UlInfo->Renderer->Render();

      ultralight::BitmapSurface* bmSurface = (ultralight::BitmapSurface*)UlInfo->View->surface();

      ultralight::RefPtr<ultralight::Bitmap> bm = bmSurface->bitmap();

      void* newPixels = bm->LockPixels();

      resX = bm->width();
      resY = bm->height();
      pixels = newPixels;

      bm->UnlockPixels();
   }
}