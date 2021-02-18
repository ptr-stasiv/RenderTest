#include "input-manager.h"

#include <algorithm>
#include <unordered_map>

namespace input
{
   using KeyCheckFunc = const bool(*)(const uint8_t);

   //This is shouldn't be changed without coordination with enum class
   static KeyCheckFunc g_KeyCheckFuncLookup[] = { native::IsPressOccured, native::IsReleaseOccured };

   struct GesturesInfo
   {
      static inline  math::Vector2 LastFrameCursorPosition;
      static inline float LastFrameScrollValue;

      static inline math::Vector2 CursorPositionDiff;
      static inline float ScrollValueDiff;
   };

   inline float GetGestureWeight(const uint8_t e)
   {
      switch (static_cast<InputEvent>(e))
      {
      case InputEvent::MouseMove_X:
         {
            return std::clamp(GesturesInfo::CursorPositionDiff.x, -1.0f, 1.0f);
         }break;
      case InputEvent::MouseMove_Y:
         {  
            return std::clamp(GesturesInfo::CursorPositionDiff.y, -1.0f, 1.0f);
         } break;
      case InputEvent::MouseScroll:
         {
            return std::clamp(GesturesInfo::ScrollValueDiff, -1.0f, 1.0f);
         } break;
      default:
         return 1.0f;
      }
   }

   static void ProcessGestures()
   {
      GesturesInfo::CursorPositionDiff = MouseInfo::CursorPosition - GesturesInfo::LastFrameCursorPosition;

      native::g_CurrentFrameEventStates[static_cast<size_t>(InputEvent::MouseMove_X)] = GesturesInfo::CursorPositionDiff.x != 0.0f;
      native::g_CurrentFrameEventStates[static_cast<size_t>(InputEvent::MouseMove_Y)] = GesturesInfo::CursorPositionDiff.y != 0.0f;

      GesturesInfo::ScrollValueDiff = MouseInfo::ScrollValue - GesturesInfo::LastFrameScrollValue;
      native::g_CurrentFrameEventStates[static_cast<size_t>(InputEvent::MouseScroll)] = GesturesInfo::ScrollValueDiff != 0.0f;

      GesturesInfo::LastFrameCursorPosition = MouseInfo::CursorPosition;
      GesturesInfo::LastFrameScrollValue = MouseInfo::ScrollValue;
   }

   void InputManager::Poll()
   {
      native::Update();
      
      ProcessGestures();

      //Sort all keys by similarities
      std::sort(ActionsKeyList.begin(), ActionsKeyList.end());
      std::sort(AxisesKeyList.begin(), AxisesKeyList.end());

      for (auto e : ActionsKeyList)
      {
         if (g_KeyCheckFuncLookup[e.DesiredStateId](e.KeyId))
            e.Callback();
      }

      for (auto e : AxisesKeyList)
      {
         float value = e.MinValue;
         if (native::IsPressed(e.KeyId))
            value = e.MaxValue * GetGestureWeight(e.KeyId);

         e.Callback(value);
      }
   }
}