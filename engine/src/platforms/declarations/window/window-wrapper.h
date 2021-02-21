#pragma once
#include <string_view>
#include <memory>

#include "math/vectors/vector2.h"

namespace platform
{
   class Window
   {
   private:
      uint16_t Width;
      uint16_t Height;
      std::string_view Title;

      struct NativeInfo;
      std::unique_ptr<NativeInfo> Native;
   public:
      Window();
      ~Window();

      bool Instantiate(const uint16_t width = 1280, const uint16_t height = 720,
                       const std::string_view title = "RenderTest");

      void BeginFrame() const;
      void EndFrame() const;

      bool ShouldClose() const;

      template<typename T>
      T* GetNative() const;

      inline const uint32_t GetWidth() const
      {
         return Width;
      }

      inline const uint32_t GetHeight() const
      {
         return Height;
      }

      Window(const Window*) = delete;
      Window(Window&&)      = delete;
      Window& operator = (const Window&) = delete;
      Window& operator = (Window&&)      = delete;
   };
}