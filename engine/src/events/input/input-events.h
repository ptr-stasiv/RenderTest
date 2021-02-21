#include "events/event.h"

namespace event
{
   class KeyEvent : public BaseEvent
   {
   public:
      uint32_t Key;
      uint32_t State;

      inline KeyEvent(const uint32_t key, const uint32_t state)
         : Key(key), State(state) {}

      ASSIGN_EVENT_TYPE(0x01)
   };

   class MouseButtonEvent : public BaseEvent
   {
   public:
      uint32_t Button;
      uint32_t State;

      inline MouseButtonEvent(const uint32_t button, const uint32_t state)
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
}