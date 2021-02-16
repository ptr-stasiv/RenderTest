#include "native-input-handling.h"
#include "glfw/glfw-input-wrapper.h"

namespace input
{
   namespace native
   {
      bgl::WindowGL* g_GlfwFocusedWindow;

      std::bitset<MaxEvents> g_CurrentFrameEventStates;
      std::bitset<MaxEvents> g_LastFrameEventStates;
      std::bitset<MaxEvents> g_ReleasedEvents;
      std::bitset<MaxEvents> g_PressedEvents;
      std::bitset<MaxEvents> g_ChangedEvents;

      //Temporal solution, I'm planning to rework this when will add joystick support
      void ProcessMouseMoveEvents()
      {
         g_CurrentFrameEventStates[static_cast<uint8_t>(InputEvent::MouseMovePos_X)]      = false;
         g_CurrentFrameEventStates[static_cast<uint8_t>(InputEvent::MouseMovePos_Y)]      = false;
         g_CurrentFrameEventStates[static_cast<uint8_t>(InputEvent::MouseMoveNeg_X)]      = false;
         g_CurrentFrameEventStates[static_cast<uint8_t>(InputEvent::MouseMoveNeg_Y)]      = false;
         g_CurrentFrameEventStates[static_cast<uint8_t>(InputEvent::MouseScroll_Up)]      = false;
         g_CurrentFrameEventStates[static_cast<uint8_t>(InputEvent::MouseScroll_Down)]    = false;

         static float x, y;
         auto v = GetCursorPosition();

         if (x != v.x)
            g_CurrentFrameEventStates[static_cast<uint8_t>((v.x - x < 0) ? InputEvent::MouseMoveNeg_X : InputEvent::MouseMovePos_X)] = true;
         if (y != v.y)
            g_CurrentFrameEventStates[static_cast<uint8_t>((v.y - y < 0) ? InputEvent::MouseMoveNeg_Y : InputEvent::MouseMovePos_Y)] = true;

         x = v.x;
         y = v.y;


         static float scroll;
         float sv = GetScrollValue();

         if (scroll != sv)
            g_CurrentFrameEventStates[static_cast<uint8_t>(sv < 0 ? InputEvent::MouseScroll_Down : InputEvent::MouseScroll_Up)] = true;

         scroll = sv;
      }

      void Update()
      {
         ProcessMouseMoveEvents();

         g_ChangedEvents = g_CurrentFrameEventStates ^ g_LastFrameEventStates;

         g_PressedEvents = g_ChangedEvents & g_CurrentFrameEventStates;
         g_ReleasedEvents = g_ChangedEvents & ~g_CurrentFrameEventStates;

         g_LastFrameEventStates = g_CurrentFrameEventStates;
      }
   }
}