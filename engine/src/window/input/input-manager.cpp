#include "input-manager.h"

#include <algorithm>
#include <unordered_map>

#include "window/window.h"

namespace input
{
   struct GesturesInfo
   {
      static inline  mm::vec2 LastFrameCursorPosition;
      static inline float LastFrameScrollValue;

      static inline mm::vec2 CursorPositionDiff;
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

   void InputManager::ProcessGestures()
   {
      GesturesInfo::CursorPositionDiff = NativeInputManager->MouseInfo.CursorPosition- GesturesInfo::LastFrameCursorPosition;

      NativeInputManager->CurrentFrameEventStates[static_cast<size_t>(InputEvent::MouseMove_X)] = GesturesInfo::CursorPositionDiff.x != 0.0f;
      NativeInputManager->CurrentFrameEventStates[static_cast<size_t>(InputEvent::MouseMove_Y)] = GesturesInfo::CursorPositionDiff.y != 0.0f;

      GesturesInfo::ScrollValueDiff = NativeInputManager->MouseInfo.ScrollValue - GesturesInfo::LastFrameScrollValue;
      NativeInputManager->CurrentFrameEventStates[static_cast<size_t>(InputEvent::MouseScroll)] = GesturesInfo::ScrollValueDiff != 0.0f;

      GesturesInfo::LastFrameCursorPosition = NativeInputManager->MouseInfo.CursorPosition;
      GesturesInfo::LastFrameScrollValue = NativeInputManager->MouseInfo.ScrollValue;
   }


   using KeyCheckFunc = const bool(*)(const uint8_t);

   void InputManager::Poll()
   {
      ProcessGestures();

      NativeInputManager->Update();

      //Sort all keys by similarities
      std::sort(ActionsKeyList.begin(), ActionsKeyList.end());
      std::sort(AxisesKeyList.begin(), AxisesKeyList.end());

      for (auto e : ActionsKeyList)
      {
         if (e.DesiredStateId == static_cast<uint8_t>(InputEventState::Pressed)
             && NativeInputManager->IsPressOccured(e.KeyId))
         {
            e.Callback(e.Args);
         }
         else if (e.DesiredStateId == static_cast<uint8_t>(InputEventState::Released)
                  && NativeInputManager->IsReleaseOccured(e.KeyId))
         {
            e.Callback(e.Args);
         }
      }

      for (auto e : AxisesKeyList)
      {
         float value = e.MinValue;
         if (NativeInputManager->IsPressed(e.KeyId))
            value = e.MaxValue * GetGestureWeight(e.KeyId);

         e.Callback(value, e.Args);
      }
   }
}