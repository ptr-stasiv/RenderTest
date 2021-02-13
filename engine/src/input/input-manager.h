#pragma once
#include <cstdint>
#include <vector>

#include "native-input-handling.h"

namespace input
{
   using ActionFunc = void(*)();
   using AxisFunc   = void(*)(const float);

   //Shouldn't be changed 
   //The values and elements are hardly connected to code in implementation file 
   enum class KeyState : uint8_t
   {
      Pressed = 1,
      Released = 0,
   };


   struct ActionCallbackInfo
   {
      uint16_t KeyId;
      uint8_t  DesiredStateId;
      ActionFunc Callback;

      inline ActionCallbackInfo(const Key key, const KeyState desiredState, const ActionFunc callback)
         : KeyId(static_cast<uint16_t>(key)), DesiredStateId(static_cast<uint8_t>(desiredState)), Callback(callback) {}

      inline bool operator < (const ActionCallbackInfo& a) const
      {
         if (DesiredStateId < a.DesiredStateId)
            return true;
         else 
            return KeyId < a.KeyId;
      }
   };

   struct AxisCallbackInfo
   {
      AxisFunc Callback;
      uint16_t KeyId;

      float MinValue;
      float MaxValue;

      inline AxisCallbackInfo(const Key key, const AxisFunc& callback, const float minValue, const float maxValue)
         : KeyId(static_cast<uint16_t>(key)), Callback(callback), MinValue(minValue), MaxValue(maxValue) {}

      inline bool operator < (const AxisCallbackInfo& a) const
      {
         return KeyId < a.KeyId;
      }
   };

   class InputManager
   {
   private:
      static inline std::vector<ActionCallbackInfo> ActionsKeyList;
      static inline std::vector<AxisCallbackInfo> AxisesKeyList;
   public:
      static void Poll();

      static inline void BindAction(const Key key, const KeyState desiredState, const ActionFunc callback)
      {
         if (static_cast<uint16_t>(key) > native::MaxKeyStates)
         {
            LOG_ERROR("Invalid bind action!");
            return;
         }

         ActionsKeyList.emplace_back(key, desiredState, callback);
      }

      static inline void BindAxis(const Key key, const AxisFunc callback, const float minValue = 0.0f, const float maxValue = 1.0f)
      {
         if (static_cast<uint16_t>(key) > native::MaxKeyStates)
         {
            LOG_ERROR("Invalid bind action!");
            return;
         }

         AxisesKeyList.emplace_back(key, callback, minValue, maxValue);
      }
   };
}