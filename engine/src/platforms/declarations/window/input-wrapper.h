#pragma once
#include <functional>

#include "events/subject.h"
#include "events/input/input-events.h"
#include "platforms/declarations/window/window-wrapper.h"

namespace platform
{
   struct InputWrapper
   {
      InputWrapper(Window* window);

      event::Subject KeyEventSubj;
      event::Subject MouseButtonEventSubj;
      event::Subject MouseCursorPosEventSubj;
      event::Subject MouseScrollEventSubj;
   };
}