#pragma once
#include <string_view>
#include <memory>

#include "math/math.h"
#include "graphics/api/devices/graphics-device.h"

namespace app
{
   //Now window is perform the role of canvas wrapper but later I'm planning to give it additional functional 

   class Window
   {
   private:
      std::shared_ptr<graphics::Canvas> Canvas;
   public:
      inline Window(const std::shared_ptr<graphics::GraphicsDevice>& device)
      {
         Canvas = device->CreateCanvas(1280, 720, "RenderTest"); //TODO values loading from config
      }

      ~Window() = default;

      Window(Window&&) = default;
      Window& operator = (Window&&) = default;

      inline void BeginFrame()
      {
         Canvas->BeginFrame();
      }

      inline void EndFrame()
      {
         Canvas->EndFrame();
      }

      bool ShouldClose()
      {
         return Canvas->ShouldClose();
      }

      const std::shared_ptr<graphics::Canvas>& GetCanvas() const
      {
         return Canvas;
      }

      Window(const Window*) = delete;
      Window& operator = (const Window&) = delete;
   };
}