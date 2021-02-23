#pragma once
#include <functional>

#include "events/subject.h"
#include "events/input/input-events.h"
#include "platforms/declarations/window/window-wrapper.h"

namespace platform
{
   namespace input
   {
      struct InputWrapper
      {
         InputWrapper(app::Window* window);

         event::Subject KeyEventSubj;
         event::Subject MouseButtonEventSubj;
         event::Subject MouseCursorPosEventSubj;
         event::Subject MouseScrollEventSubj;
      };
   }
}