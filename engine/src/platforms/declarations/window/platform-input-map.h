#pragma once
#include <unordered_map>
#include "input/input-event-codes.h"

namespace platform
{
   namespace input
   {
      extern std::unordered_map<uint8_t, ::input::InputEvent> PlatformInputMap;
   }
}