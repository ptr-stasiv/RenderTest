#pragma once
#include <cstdint>

namespace event
{
#define ASSIGN_EVENT_TYPE(type) \
   virtual uint8_t GetType() const { return type; } \
   static uint8_t GetStaticType() { return type; }

   class BaseEvent
   {
   public:
      ASSIGN_EVENT_TYPE(0x00)
   };
}