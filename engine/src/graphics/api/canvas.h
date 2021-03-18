#pragma once
#include <cstdint>
#include <any>

#include "events/subject.h"

namespace graphics
{
   class Canvas
   {
   public:
      Canvas() = default;
      virtual ~Canvas() = default;

      virtual void BeginFrame() = 0;
      virtual void EndFrame() = 0;

      virtual bool ShouldClose() = 0;

      virtual uint16_t GetWidth() const = 0;
      virtual uint16_t GetHeight() const = 0;

      virtual void AddKeyCallback(const event::Callback& callback) = 0;
      virtual void AddMouseButtonCallback(const event::Callback& callback) = 0;
      virtual void AddCursorCallback(const event::Callback& callback) = 0;
      virtual void AddScrollCallback(const event::Callback& callback) = 0;
   };
}
