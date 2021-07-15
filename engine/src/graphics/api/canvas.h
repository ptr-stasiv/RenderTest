#pragma once
#include <cstdint>
#include <any>

#include "events/subject.h"
#include "imgui.h"

namespace graphics
{
   class Canvas
   {
   public:
      Canvas() = default;
      virtual ~Canvas() = default;

      virtual void BeginFrame() = 0;
      virtual void EndFrame() = 0;

      virtual bool ShouldClose() const = 0;

      virtual void ShowCursor(const bool show) = 0;

      virtual uint16_t GetWidth() const = 0;
      virtual uint16_t GetHeight() const = 0;

      virtual void AddKeyCallback(const event::Callback& callback) = 0;
      virtual void AddMouseButtonCallback(const event::Callback& callback) = 0;
      virtual void AddCursorCallback(const event::Callback& callback) = 0;
      virtual void AddScrollCallback(const event::Callback& callback) = 0;
      virtual void AddResizeCallback(const event::Callback& callback) = 0;
   };
}
