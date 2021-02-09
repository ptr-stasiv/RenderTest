#include "gui.h"

#include "Ultralight/Ultralight.h"
#include "AppCore/Platform.h"

#include <stdio.h>
#include <string>

namespace gui
{
   struct UlInfoPimpl
   {
      ultralight::RefPtr<ultralight::Renderer> Renderer;
      ultralight::RefPtr<ultralight::View> View;
   };

   GuiController::GuiController()
      : UlInfo(new UlInfoPimpl) {}

   GuiController::~GuiController() = default;

   void GuiController::Setup(const uint32_t resX, const uint32_t resY)
   {
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