#pragma once
#include <cstdint>
#include <bitset>

#include "window/window.h"
#include "events/subject.h"
#include "events/input-events.h"
#include "window/input/input-events.h"
#include "debug/globals.h"
#include "math/vectors/vector2.h"

namespace input
{
   namespace native
   {
      inline constexpr uint8_t MaxEvents = (uint8_t)(input::InputEvent::LAST_ENUM_ELEMENT);

      class NativeInputManager
      {
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
            uintptr_t args = reinterpret_cast<uintptr_t>(this);

            event::Callback keyCallback = { [](event::BaseEvent& e, const uintptr_t args)
            {
               NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
               event::KeyEvent keyE = event::CastEvent<event::KeyEvent>(e);

               native->CurrentFrameEventStates[static_cast<size_t>(keyE.Key)] = keyE.State == InputEventState::Pressed;
            }, args };

            event::Callback mouseButtonCallback = { [](event::BaseEvent& e, const uintptr_t args)
            {
               NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
               event::MouseButtonEvent mouseButtonE = event::CastEvent<event::MouseButtonEvent>(e);

               size_t pos = static_cast<size_t>(1 + size_t(mouseButtonE.Button) + size_t(input::InputEvent::LastKeyboardKey));
               native->CurrentFrameEventStates[pos] = mouseButtonE.State == InputEventState::Pressed;
            }, args };

            event::Callback cursorCallback = { [](event::BaseEvent& e, const uintptr_t args)
            {
               NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
               event::MouseCursorPosEvent mouseCursorE = event::CastEvent<event::MouseCursorPosEvent>(e);
            
               native->MouseInfo.CursorPosition = { mouseCursorE.PosX, mouseCursorE.PosY };
            }, args };

            event::Callback mouseScrollCallback = { [](event::BaseEvent& e, const uintptr_t args)
            {
               NativeInputManager* native = reinterpret_cast<NativeInputManager*>(args);
               event::MouseScrollEvent mouseScrollE = event::CastEvent<event::MouseScrollEvent>(e);

               native->MouseInfo.ScrollValue = mouseScrollE.Value;
            }, args };

            window->GetCanvas()->AddKeyCallback(keyCallback);
            window->GetCanvas()->AddMouseButtonCallback(mouseButtonCallback);
            window->GetCanvas()->AddCursorCallback(cursorCallback);
            window->GetCanvas()->AddScrollCallback(mouseScrollCallback);
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