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


      struct
      {
         float cursorX;
         float cursorY;

         float mouseScroll;
      } g_InputValuesStorage;

      void Update()
      {
         g_ChangedEvents = g_CurrentFrameEventStates ^ g_LastFrameEventStates;

         g_PressedEvents = g_ChangedEvents & g_CurrentFrameEventStates;
         g_ReleasedEvents = g_ChangedEvents & ~g_CurrentFrameEventStates;

         g_LastFrameEventStates = g_CurrentFrameEventStates;
      }
   }
}