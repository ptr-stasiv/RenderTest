#include "input-manager.h"

#include <algorithm>
#include <unordered_map>

namespace input
{
   using KeyCheckFunc = const bool(*)(const uint8_t);

   //This is shouldn't be changed without coordination with enum class
   static KeyCheckFunc g_KeyCheckFuncLookup[] = { native::IsPressOccured, native::IsReleaseOccured };

   void InputManager::Poll()
   {
      native::Update();

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
            value = e.MaxValue;

         e.Callback(value);
      }
   }
}