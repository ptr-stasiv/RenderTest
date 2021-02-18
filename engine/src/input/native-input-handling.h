#pragma once
#include <cstdint>
#include <bitset>

#include "bgl/window/window-wrapper.h"
#include "glfw/glfw-key-remapping.h"
#include "debug/log/log.h"
#include "math/vectors/vector2.h"

namespace input
{
   namespace native
   {
     extern bgl::WindowGL* g_GlfwFocusedWindow;

      //
      //'Event' mean all events that can be done with input devices like keyboard key press or mouse move
      //

      static constexpr uint8_t MaxEvents = (uint8_t)(input::InputEvent::LAST_ENUM_ELEMENT);

      extern std::bitset<MaxEvents> g_CurrentFrameEventStates;
      extern std::bitset<MaxEvents> g_LastFrameEventStates;
      extern std::bitset<MaxEvents> g_ReleasedEvents;
      extern std::bitset<MaxEvents> g_PressedEvents;
      extern std::bitset<MaxEvents> g_ChangedEvents;

      inline void Update()
      {
         g_ChangedEvents = g_CurrentFrameEventStates ^ g_LastFrameEventStates;

         g_PressedEvents = g_ChangedEvents & g_CurrentFrameEventStates;
         g_ReleasedEvents = g_ChangedEvents & ~g_CurrentFrameEventStates;

         g_LastFrameEventStates = g_CurrentFrameEventStates;
      }

      //This functions should be called somewhere to make input manager work
      namespace callback
      {
         static inline void OnKeyStateChanged(uint32_t key, uint32_t sc, uint32_t action, uint32_t mods)
         {
            g_CurrentFrameEventStates[static_cast<size_t>(mapping::g_GlfwInputEventMap[key])] = (action != GLFW_RELEASE) ? true : false;
         }

         static inline void OnMouseButtonStateChanged(uint32_t button, uint32_t action, uint32_t mods)
         {
            g_CurrentFrameEventStates[static_cast<size_t>(mapping::g_GlfwInputEventMap[1 + button + (size_t)(input::InputEvent::LastKeyboardKey)])] = (action != GLFW_RELEASE) ? true : false;
         }
      };


      //
      //All function below don't have any checks for argument validity for purpose
      //They shouldn't be used without wrapper
      //

      inline const bool IsPressOccured(const uint8_t keyId) 
      {
         return g_PressedEvents.test(keyId);
      }

      inline const bool IsReleaseOccured(const uint8_t keyId)
      {
         return g_ReleasedEvents.test(keyId);
      }

      inline const bool IsPressed(const uint8_t keyId)
      {
         return g_CurrentFrameEventStates.test(keyId);
      }
   }
}