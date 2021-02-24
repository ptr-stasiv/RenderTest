#pragma once
#include <cstdint>
#include <bitset>

#include "platforms/declarations/window/platform-input-map.h"
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
      private:
         std::shared_ptr<platform::input::InputWrapper> InputWrapperManager;

      public:

         struct
         {
            math::Vector2 CursorPosition;
            float ScrollValue;
         } MouseInfo;

         //
         //'Event' mean all events that can be done with input devices like keyboard key press or mouse move
         //
         std::bitset<MaxEvents> CurrentFrameEventStates;
         std::bitset<MaxEvents> LastFrameEventStates;
         std::bitset<MaxEvents> ReleasedEvents;
         std::bitset<MaxEvents> PressedEvents;
         std::bitset<MaxEvents> ChangedEvents;

         inline NativeInput(platform::input::InputWrapper* inputWrapper)
            : InputWrapperManager(inputWrapper)
         {
            uintptr_t args = reinterpret_cast<uintptr_t>(this);

            InputWrapperManager->KeyEventSubj.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInput* native = reinterpret_cast<NativeInput*>(args);
                  event::KeyEvent keyE = event::CastEvent<event::KeyEvent>(e);

                  native->CurrentFrameEventStates[static_cast<size_t>(platform::input::PlatformInputMap[keyE.Key])] = platform::input::TranslatePlatformState(keyE.State) == InputEventState::Pressed;
               }, args);

            InputWrapperManager->MouseButtonEventSubj.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInput* native = reinterpret_cast<NativeInput*>(args);
                  event::MouseButtonEvent mouseButtonE = event::CastEvent<event::MouseButtonEvent>(e);

                  size_t pos = static_cast<size_t>(platform::input::PlatformInputMap[1 + mouseButtonE.Button + (size_t)(input::InputEvent::LastKeyboardKey)]);
                  native->CurrentFrameEventStates[pos] = platform::input::TranslatePlatformState(mouseButtonE.State) == InputEventState::Pressed;
               }, args);

            InputWrapperManager->MouseCursorPosEventSubj.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInput* native = reinterpret_cast<NativeInput*>(args);
                  event::MouseCursorPosEvent mouseCursorE = event::CastEvent<event::MouseCursorPosEvent>(e);

                  native->MouseInfo.CursorPosition = { mouseCursorE.PosX, mouseCursorE.PosY };
               }, args);

            InputWrapperManager->MouseScrollEventSubj.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInput* native = reinterpret_cast<NativeInput*>(args);
                  event::MouseScrollEvent mouseScrollE = event::CastEvent<event::MouseScrollEvent>(e);

                  native->MouseInfo.ScrollValue = mouseScrollE.Value;
               }, args);
         }

         inline void Update()
         {
            ChangedEvents = CurrentFrameEventStates ^ LastFrameEventStates;

            PressedEvents = ChangedEvents & CurrentFrameEventStates;
            ReleasedEvents = ChangedEvents & ~CurrentFrameEventStates;

            LastFrameEventStates = CurrentFrameEventStates;
         }

         inline const std::shared_ptr<platform::input::InputWrapper> GetInputWrapper() const
         {
            return InputWrapperManager;
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
      };
   }
}