#pragma once
#include <unordered_map>

#include "Ultralight/Ultralight.h"
#include "input/input-event-codes.h"

namespace input
{
   static std::unordered_map<InputEvent, size_t> g_Ultralight =
   {
      { InputEvent::M_Left,   ultralight::MouseEvent::kButton_Left},
      { InputEvent::M_Middle, ultralight::MouseEvent::kButton_Middle},
      { InputEvent::M_Right,  ultralight::MouseEvent::kButton_Right}
   };
}