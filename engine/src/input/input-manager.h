#pragma once
#include <cstdint>
#include <vector>
#include <deque>
#include <utility>

#include "native-input-handling.h"

namespace input
{
   constexpr size_t MaxGestures = (size_t)(InputEvent::LastMouseGesture)-(size_t)(InputEvent::LastMouseButton);

   using ActionFunc = void(*)(const uintptr_t args);
   using AxisFunc   = void(*)(const float, const uintptr_t args);

   template<typename T>
   class BasicCallbackInfo
   {
   public:
      uint8_t KeyId;
      T Callback;

      uintptr_t Args;

      inline BasicCallbackInfo(const uint8_t keyId, const T& callback, const uintptr_t args)
         : KeyId(keyId), Callback(callback), Args(args) {}
   };
   struct ActionCallbackInfo : public BasicCallbackInfo<ActionFunc>
   {
      uint8_t DesiredStateId;

      inline ActionCallbackInfo(const InputEvent key, const InputEventState desiredState, const ActionFunc callback, const uintptr_t args)
         : BasicCallbackInfo(static_cast<uint8_t>(key), callback, args), DesiredStateId(static_cast<uint8_t>(desiredState)) {}

      inline bool operator < (const ActionCallbackInfo& a) const
      {
         if (DesiredStateId < a.DesiredStateId)
            return true;
         else 
            return KeyId < a.KeyId;
      }
   };

   struct AxisCallbackInfo : public BasicCallbackInfo<AxisFunc>
   {
      float MinValue;
      float MaxValue;

      inline AxisCallbackInfo(const InputEvent key, const AxisFunc& callback, const uintptr_t args, const float minValue, const float maxValue)
         : BasicCallbackInfo(static_cast<uint8_t>(key), callback, args), MinValue(minValue), MaxValue(maxValue) {}

      inline bool operator < (const AxisCallbackInfo& a) const
      {
         return KeyId < a.KeyId;
      }
   };

   class InputManager
   {
   private:
      std::vector<ActionCallbackInfo> ActionsKeyList;
      std::vector<AxisCallbackInfo> AxisesKeyList;

      std::unordered_map<std::string_view, std::deque<InputEvent>> ActionKeyMap;
      std::unordered_map<std::string_view, std::deque<std::pair<InputEvent, float>>> AxisKeyMap;

      std::shared_ptr<input::native::NativeInput> NativeInputManager;
   public:

      inline InputManager(input::native::NativeInput* nativeInput)
         : NativeInputManager(nativeInput) {}

      void Poll();

      inline void AddActionMapping(const std::string_view& actionName, const InputEvent key)
      {
         if (static_cast<uint8_t>(key) > native::MaxEvents)
         {
            LOG_ERROR("Invalid key specified!");
            return;
         }

         ActionKeyMap[actionName].push_back(key);
      }

      inline void AddAxisMapping(const std::string_view& actionName, const std::pair<InputEvent, float>& keyInfo)
      {
         if (static_cast<uint8_t>(keyInfo.first) > native::MaxEvents)
         {
            LOG_ERROR("Invalid key specified!");
            return;
         }

         AxisKeyMap[actionName].emplace_back(keyInfo);
      }

      inline void AddActionMapping(const std::string_view& actionName, const std::initializer_list<InputEvent>& inputEvents)
      {
         for (auto e : inputEvents)
            AddActionMapping(actionName, e);
      }
      
      inline void AddAxisMapping(const std::string_view& axisName, const std::initializer_list<std::pair<InputEvent, float>>& inputEvents)
      {
         for (auto e : inputEvents)
            AddAxisMapping(axisName, e);
      }

      inline void BindAction(const std::string_view& actionName, const InputEventState desiredState, const ActionFunc callback, const uintptr_t args = 0)
      {
         auto keyHandle = ActionKeyMap.find(actionName);
         if (keyHandle == ActionKeyMap.end())
         {
            LOG_ERROR("Invalid action name specified!");
            return;
         }

         for (auto key : keyHandle->second)
            ActionsKeyList.emplace_back(key, desiredState, callback, args);
      }

      inline void BindAction(const InputEvent& key, const InputEventState desiredState, const ActionFunc callback, const uintptr_t args = 0)
      {
         if (static_cast<uint8_t>(key) > native::MaxEvents)
         {
            LOG_ERROR("Invalid key specified!");
            return;
         }
            
         ActionsKeyList.emplace_back(key, desiredState, callback, args);
      }

      inline void BindAxis(const std::string_view& axisNamey, const AxisFunc callback, const uintptr_t args = 0, const float defaultValue = 0.0f)
      {
         auto keyHandle = AxisKeyMap.find(axisNamey);
         if (keyHandle == AxisKeyMap.end())
         {
            LOG_ERROR("Invalid axis name specified!");
            return;
         }

         for (auto keyInfo : keyHandle->second)
            AxisesKeyList.emplace_back(keyInfo.first, callback, args, defaultValue, keyInfo.second);
      }

      inline const std::shared_ptr<native::NativeInput> GetNativeInput() const
      {
         return NativeInputManager;
      }

      inline const std::shared_ptr<input::InputWrapper> GetInputWrapper() const
      {
         return NativeInputManager->GetInputWrapper();
      }
   private:
      void ProcessGestures();
   };
}