#include "input-manager.h"

#include <algorithm>

namespace input
{
   void InputManager::Poll()
   {
      native::Update();

      std::sort(ActionsKeyList.begin(), ActionsKeyList.end());
      std::sort(AxisesKeyList.begin(), AxisesKeyList.end());

      for (auto e : ActionsKeyList)
      {
         if (native::IsKeyPressOccur(static_cast<Key>(e.KeyId)))
            e.Callback();
      }

      for (auto e : AxisesKeyList)
      {
         float value = e.MinValue;
         if (native::IsKeyPressed(static_cast<Key>(e.KeyId)))
            value = e.MaxValue;

         e.Callback(value);
      }
   }
}