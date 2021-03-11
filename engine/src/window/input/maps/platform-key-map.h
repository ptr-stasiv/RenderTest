#pragma once
#include <unordered_map>
#include "window/input/maps/input-events.h"

namespace input
{
   extern std::unordered_map<uint32_t, ::input::InputEvent> PlatformInputMap;

   extern ::input::InputEventState TranslatePlatformState(const uint32_t state);
}