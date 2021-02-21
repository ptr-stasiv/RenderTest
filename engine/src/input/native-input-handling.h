#pragma once
#include <cstdint>
#include <bitset>

#include "glfw/glfw-event-mapping.h"
#include "debug/log/log.h"
#include "math/vectors/vector2.h"
#include "platforms/declarations/window/input-wrapper.h"

namespace input
{
   namespace native
   {
      static constexpr uint8_t MaxEvents = (uint8_t)(input::InputEvent::LAST_ENUM_ELEMENT);

      class NativeInput
      {
      public:
         //
         //'Event' mean all events that can be done with input devices like keyboard key press or mouse move
         //
         std::bitset<MaxEvents> CurrentFrameEventStates;
         std::bitset<MaxEvents> LastFrameEventStates;
         std::bitset<MaxEvents> ReleasedEvents;
         std::bitset<MaxEvents> PressedEvents;
         std::bitset<MaxEvents> ChangedEvents;

         inline NativeInput(platform::InputWrapper& inputWrapper)
         {
         }

         inline void Update()
         {
            ChangedEvents = CurrentFrameEventStates ^ LastFrameEventStates;

            PressedEvents = ChangedEvents & CurrentFrameEventStates;
            ReleasedEvents = ChangedEvents & ~CurrentFrameEventStates;

            LastFrameEventStates = CurrentFrameEventStates;
         }


         //
         //All function below don't have any checks for argument validity for purpose
         //They shouldn't be used without wrapper
         //

         inline const bool IsPressOccured(const uint8_t keyId)
         {
            return PressedEvents.test(keyId);
         }

         inline const bool IsReleaseOccured(const uint8_t keyId)
         {
            return ReleasedEvents.test(keyId);
         }

         inline const bool IsPressed(const uint8_t keyId)
         {
            return CurrentFrameEventStates.test(keyId);
         }
      private:
         inline void OnKeyEvent(event::BaseEvent& e)
         {

         }

         inline void OnMouseButtonEvent(event::BaseEvent& e)
         {

         }

         inline void OnMouseCursorEvent(event::BaseEvent& e)
         {

         }

         inline void OnMouseScrollEvent(event::BaseEvent& e)
         {

         }
      };




      //This functions should be called somewhere to make input manager work
      namespace callback
      {
         static inline void OnKeyStateChanged(uint32_t key, uint32_t sc, uint32_t action, uint32_t mods)
         {
            CurrentFrameEventStates[static_cast<size_t>(mapping::GlfwInputEventMap[key])] = (action != GLFW_RELEASE) ? true : false;
         }

         static inline void OnMouseButtonStateChanged(uint32_t button, uint32_t action, uint32_t mods)
         {
            CurrentFrameEventStates[static_cast<size_t>(mapping::GlfwInputEventMap[1 + button + (size_t)(input::InputEvent::LastKeyboardKey)])] = (action != GLFW_RELEASE) ? true : false;
         }
      };





   }
}