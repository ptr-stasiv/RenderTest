#pragma once
#include <functional>

#include "events/subject.h"
#include "events/input/input-events.h"
#include "window/window.h"

namespace input
{
   namespace native
   {
      void SetupInputCallbacks(const std::shared_ptr<app::Window>& window);

      void AddKeyCallback(const event::Subject& keySubject);
      void AddMouseButtonCallback(const event::Subject& mouseSubject);
      void AddCursorCallback(const event::Subject& cursorSubject);
      void AddScrollCallback(const event::Subject& scrollCallback);
   }
}