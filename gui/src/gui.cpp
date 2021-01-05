#include "gui.h"

#include "Ultralight/Ultralight.h"
#include "AppCore/Platform.h"

#include <stdio.h>

static ultralight::RefPtr<ultralight::Renderer> g_Renderer;
static ultralight::RefPtr<ultralight::View> g_View;

void InitializeGUI(const char* url, const int width, const int height)
{
   ultralight::Config config;

   config.resource_path = "./resources/";

   config.use_gpu_renderer = false;

   config.device_scale = 1.0f;

   ultralight::Platform::instance().set_config(config);


   //UL Platform
   ultralight::Platform::instance().set_font_loader(ultralight::GetPlatformFontLoader());

   ultralight::Platform::instance().set_file_system(ultralight::GetPlatformFileSystem("."));

   ultralight::Platform::instance().set_logger(ultralight::GetDefaultLogger("ultralight.log"));


   //UL renderer  
   g_Renderer = ultralight::Renderer::Create();

   g_View = g_Renderer->CreateView(width, height, false, nullptr);

   g_View->LoadURL(url);

   g_View->Focus();
} 

void GetWebWindowInfo(int* width, int* height, void** winPixels)
{
   g_Renderer->Update();
   g_Renderer->Render();

   ultralight::BitmapSurface* bmSurface = (ultralight::BitmapSurface*)g_View->surface();

   ultralight::RefPtr<ultralight::Bitmap> bm = bmSurface->bitmap();

   void* pixels = bm->LockPixels();

   *width   = bm->width();
   *height  = bm->height();
   *winPixels = pixels;

   bm->UnlockPixels();
}