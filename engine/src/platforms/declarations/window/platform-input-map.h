#pragma once
#include <unordered_map>
#include "input/input-event-codes.h"

namespace platform
{
   namespace input
   {
      extern std::unordered_map<uint32_t, ::input::InputEvent> PlatformInputMap;
      extern ::input::InputEventState TranslatePlatformState(const uint32_t state);
   }
}