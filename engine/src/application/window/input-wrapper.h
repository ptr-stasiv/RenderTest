#pragma once
#include <functional>

#include "events/subject.h"
#include "events/input/input-events.h"
#include "window-wrapper.h"

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