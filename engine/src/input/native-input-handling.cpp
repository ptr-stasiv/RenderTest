#include "native-input-handling.h"

namespace input
{
   namespace native
   {
      std::bitset<MaxKeyStates> g_CurrentFrameKeyStates;
      std::bitset<MaxKeyStates> g_LastFrameKeyStates;
      std::bitset<MaxKeyStates> g_ReleasedKeys;
      std::bitset<MaxKeyStates> g_PressedKeys;
      std::bitset<MaxKeyStates> g_ChangedKeys;
   }
}