#pragma once
#include <cstdint>
#include <vector>
#include <deque>
#include <utility>

#include "native-input-handling.h"
#include "glfw/glfw-input-wrapper.h"

namespace input
{
   using ActionFunc = void(*)();
   using AxisFunc   = void(*)(const float);

   //Shouldn't be changed 
   //The values and elements are hardly connected to code in implementation file 
   enum class InputEventState : uint8_t
   {
      Pressed = 1,
      Released = 0,
   };


   struct ActionCallbackInfo
   {
      uint8_t KeyId;
      uint8_t DesiredStateId;
      ActionFunc Callback;

      inline ActionCallbackInfo(const InputEvent key, const InputEventState desiredState, const ActionFunc callback)
         : KeyId(static_cast<uint8_t>(key)), DesiredStateId(static_cast<uint8_t>(desiredState)), Callback(callback) {}

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
      uint8_t KeyId;

      float MinValue;
      float MaxValue;

      inline AxisCallbackInfo(const InputEvent key, const AxisFunc& callback, const float minValue, const float maxValue)
         : KeyId(static_cast<uint8_t>(key)), Callback(callback), MinValue(minValue), MaxValue(maxValue) {}

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

      static inline std::unordered_map<std::string_view, std::deque<InputEvent>> ActionKeyMap;
      static inline std::unordered_map<std::string_view, std::deque<std::pair<InputEvent, float>>> AxisKeyMap;
   public:
      static void Poll();

      template<typename T, typename ...Args>
      static inline void AddActionMapping(const std::string_view& actionName, const T key, const Args... keys)
      {
         AddActionMappin(actionName, key);
         AddActionMapping(actionName, keys...);
      }

      template<typename K, typename ...Args>
      static inline void AddAxisMapping(const std::string_view& axisName, const K& keyInfo, const Args&... keys)
      {
         AddAxisMapping(axisName, keyInfo);
         AddAxisMapping(axisName, keys...);
      }

      static inline void BindAction(const std::string_view& actionName, const InputEventState desiredState, const ActionFunc callback)
      {
         auto keyHandle = ActionKeyMap.find(actionName);
         if (keyHandle == ActionKeyMap.end())
         {
            LOG_ERROR("Invalid action name specified!");
            return;
         }

         for (auto key : keyHandle->second)
            ActionsKeyList.emplace_back(key, desiredState, callback);
      }

      static inline void BindAction(const InputEvent& key, const InputEventState desiredState, const ActionFunc callback)
      {
         if (static_cast<uint8_t>(key) > native::MaxEvents)
         {
            LOG_ERROR("Invalid key specified!");
            return;
         }
            
         ActionsKeyList.emplace_back(key, desiredState, callback);
      }

      static inline void BindAxis(const std::string_view& axisNamey, const AxisFunc callback, const float defaultValue = 0.0f)
      {
         auto keyHandle = AxisKeyMap.find(axisNamey);
         if (keyHandle == AxisKeyMap.end())
         {
            LOG_ERROR("Invalid axis name specified!");
            return;
         }

         for (auto keyInfo : keyHandle->second)
            AxisesKeyList.emplace_back(keyInfo.first, callback, defaultValue, keyInfo.second);
      }
   private:
      static inline void AddActionMapping(const std::string_view& actionName, const InputEvent key)
      {
         if (static_cast<uint8_t>(key) > native::MaxEvents)
         {
            LOG_ERROR("Invalid key specified!");
            return;
         }

         ActionKeyMap[actionName].push_back(key);
      }

      static inline void AddAxisMapping(const std::string_view& actionName, const std::pair<InputEvent, float>& keyInfo)
      {
         if (static_cast<uint8_t>(keyInfo.first) > native::MaxEvents)
         {
            LOG_ERROR("Invalid key specified!");
            return;
         }

         AxisKeyMap[actionName].emplace_back(keyInfo);
      }
   };
}