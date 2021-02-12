#pragma once
#include <cstdint>
#include <bitset>

#include "bgl/window/window-wrapper.h"
#include "glfw-key-remapping.h"
#include "debug/log/log.h"

namespace input
{
   namespace native
   {
      static bgl::WindowGL* g_FocusedWindow;

      static constexpr uint16_t MaxKeyStates = static_cast<uint16_t>(input::Key::LAST_ENUM_ELEMENT);
      extern std::bitset<MaxKeyStates> g_CurrentFrameKeyStates;
      extern std::bitset<MaxKeyStates> g_LastFrameKeyStates;
      extern std::bitset<MaxKeyStates> g_ReleasedKeys;
      extern std::bitset<MaxKeyStates> g_PressedKeys;
      extern std::bitset<MaxKeyStates> g_ChangedKeys;

      inline void GlfwKeyCallback(GLFWwindow* window, int key, int sc, int action, int mods)
      {
         g_CurrentFrameKeyStates[static_cast<uint16_t>(mapping::g_GlfwKeyMap[key])] = (action != GLFW_RELEASE) ? true : false;
      }

      inline void Update()
      {
         g_ChangedKeys  = g_CurrentFrameKeyStates ^ g_LastFrameKeyStates;

         g_PressedKeys  = g_ChangedKeys & g_CurrentFrameKeyStates;
         g_ReleasedKeys = g_ChangedKeys & ~g_CurrentFrameKeyStates;

         g_LastFrameKeyStates = g_CurrentFrameKeyStates;
      }

      inline const bool IsKeyPressOccur(const Key key) //Key here for purpose isn't checked for validity this will be done in the wrapper
      {
         return g_PressedKeys.test(static_cast<uint16_t>(key));
      }

      inline const bool IsKeyReleaseOccur(const Key key)
      {
         return g_ReleasedKeys.test(static_cast<size_t>(key));
      }

      inline const bool IsKeyPressed(const Key key)
      {
         return g_CurrentFrameKeyStates.test(static_cast<uint16_t>(key));
      }
   }

   inline void SetWindowFocus(bgl::WindowGL& window)
   {
      native::g_FocusedWindow = &window;

      glfwSetKeyCallback(native::g_FocusedWindow->NativeHandle.get(), native::GlfwKeyCallback);
   }
}