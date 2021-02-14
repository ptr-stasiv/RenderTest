#include "input-manager.h"

#include <algorithm>
#include <unordered_map>

namespace input
{
   using KeyCheckFunc = const bool(*)(const Key);

   //This is shouldn't be changed without coordination with enum class
   static KeyCheckFunc g_KeyCheckFuncLookup[] = { native::IsKeyReleaseOccur, native::IsKeyPressOccur };

   void InputManager::Poll()
   {
      native::Update();

      //Sort all keys by similarities
      std::sort(ActionsKeyList.begin(), ActionsKeyList.end());
      std::sort(AxisesKeyList.begin(), AxisesKeyList.end());

      for (auto e : ActionsKeyList)
      {
         if (g_KeyCheckFuncLookup[e.DesiredStateId](static_cast<Key>(e.KeyId)))
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