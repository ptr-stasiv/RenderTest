#pragma once
#include <cstdint>
#include <bitset>

#include "window/input/maps/platform-key-map.h"
#include "debug/log/log.h"
#include "math/vectors/vector2.h"
#include "window/input/platform-input.h"

namespace input
{
   namespace native
   {
      static constexpr uint8_t MaxEvents = (uint8_t)(input::InputEvent::LAST_ENUM_ELEMENT);

      class NativeInputManager
      {
      private:
         event::Subject KeySubject;
         event::Subject MouseButtonSubject;
         event::Subject CursorSubject;
         event::Subject ScrollSubject;
      public:
         struct
         {
            math::Vector2 CursorPosition;
            float ScrollValue;
         } MouseInfo = { 0 };


         std::bitset<MaxEvents> CurrentFrameEventStates;
         std::bitset<MaxEvents> LastFrameEventStates;
         std::bitset<MaxEvents> ReleasedEvents;
         std::bitset<MaxEvents> PressedEvents;
         std::bitset<MaxEvents> ChangedEvents;

         inline NativeInputManager(const std::shared_ptr<app::Window>& window)
         {
            native::SetupInputCallbacks(window);


            uintptr_t args = reinterpret_cast<uintptr_t>(this);

            KeySubject.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
                  event::KeyEvent keyE = event::CastEvent<event::KeyEvent>(e);

                  native->CurrentFrameEventStates[static_cast<size_t>(input::PlatformInputMap[keyE.Key])] = input::TranslatePlatformState(keyE.State) == InputEventState::Pressed;
               }, args);

            MouseButtonSubject.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
                  event::MouseButtonEvent mouseButtonE = event::CastEvent<event::MouseButtonEvent>(e);

                  size_t pos = static_cast<size_t>(input::PlatformInputMap[1 + mouseButtonE.Button + (size_t)(input::InputEvent::LastKeyboardKey)]);
                  native->CurrentFrameEventStates[pos] = input::TranslatePlatformState(mouseButtonE.State) == InputEventState::Pressed;
               }, args);

            CursorSubject.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
                  event::MouseCursorPosEvent mouseCursorE = event::CastEvent<event::MouseCursorPosEvent>(e);

                  native->MouseInfo.CursorPosition = { mouseCursorE.PosX, mouseCursorE.PosY };
               }, args);

            ScrollSubject.AddObserver([](event::BaseEvent& e, const uintptr_t args)
               {
                  NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
                  event::MouseScrollEvent mouseScrollE = event::CastEvent<event::MouseScrollEvent>(e);

                  native->MouseInfo.ScrollValue = mouseScrollE.Value;
               }, args);


            native::AddKeyCallback(KeySubject);
            native::AddMouseButtonCallback(MouseButtonSubject);
            native::AddCursorCallback(CursorSubject);
            native::AddScrollCallback(ScrollSubject);
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
      };
   }
}