#pragma once
#include "events/event.h"
#include "window/input/input-events.h"

namespace event
{
   class KeyEvent : public BaseEvent
   {
   public:
      input::InputEvent Key;
      input::InputEventState State;

      inline KeyEvent(const input::InputEvent key, const input::InputEventState state)
         : Key(key), State(state) {}

      ASSIGN_EVENT_TYPE(0x01)
   };

   class MouseButtonEvent : public BaseEvent
   {
   public:
      input::InputEvent Button;
      input::InputEventState State;

      inline MouseButtonEvent(const input::InputEvent button, const input::InputEventState state)
         : Button(button), State(state) {}

      ASSIGN_EVENT_TYPE(0x02)
   };

   class MouseCursorPosEvent : public BaseEvent
   {
   public:
      float PosX;
      float PosY;

      inline MouseCursorPosEvent(const float posX, const float posY)
         : PosX(posX), PosY(posY) {}

      ASSIGN_EVENT_TYPE(0x03)
   };

   class MouseScrollEvent : public BaseEvent
   {
   public:
      float Value;

      inline MouseScrollEvent(const float value)
         : Value(value) {}

      ASSIGN_EVENT_TYPE(0x04)
   };

   class ResizeEvent : public BaseEvent
   {
   public:
      uint16_t Width;
      uint16_t Height;

      inline ResizeEvent(const uint16_t w, const uint16_t h)
         : Width(w), Height(h) {}

      ASSIGN_EVENT_TYPE(0x05)
   };
}