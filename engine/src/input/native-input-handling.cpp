#include "native-input-handling.h"

namespace input
{
   namespace native
   {
      std::bitset<MaxEvents> g_CurrentFrameEventStates;
      std::bitset<MaxEvents> g_LastFrameEventStates;
      std::bitset<MaxEvents> g_ReleasedEvents;
      std::bitset<MaxEvents> g_PressedEvents;
      std::bitset<MaxEvents> g_ChangedEvents;
   }
}