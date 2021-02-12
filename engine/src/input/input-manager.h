#pragma once
#include <cstdint>
#include <vector>

#include "native-input-handling.h"

namespace input
{
   using ActionFunc = void(*)();
   using AxisFunc   = void(*)(const float);

   struct ActionCallbackInfo
   {
      ActionFunc Callback;
      uint16_t KeyId;

      inline ActionCallbackInfo(const Key key, const ActionFunc callback)
         : KeyId(static_cast<uint16_t>(key)), Callback(callback) {}

      inline bool operator < (const ActionCallbackInfo& a) const
      {
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

      static inline void BindAction(const Key key, const ActionFunc callback)
      {
         if (static_cast<uint16_t>(key) > native::MaxKeyStates)
         {
            LOG_ERROR("Invalid bind action!");
            return;
         }

         ActionsKeyList.emplace_back(key, callback);
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