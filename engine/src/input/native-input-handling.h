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

      //
      //'Key' impersonate all keys, buttons of the input devices, in other words all things that can be pressed and released
      //

      static constexpr uint8_t MaxKeyStates = (uint8_t)(input::InputKey::LAST_ENUM_ELEMENT);

      extern std::bitset<MaxKeyStates> g_CurrentFrameKeyStates;
      extern std::bitset<MaxKeyStates> g_LastFrameKeyStates;
      extern std::bitset<MaxKeyStates> g_ReleasedKeys;
      extern std::bitset<MaxKeyStates> g_PressedKeys;
      extern std::bitset<MaxKeyStates> g_ChangedKeys;

      inline void GlfwKeyCallback(GLFWwindow* window, int key, int sc, int action, int mods)
      {
         g_CurrentFrameKeyStates[static_cast<uint8_t>(mapping::g_GlfwInputKeyMap[key])] = (action != GLFW_RELEASE) ? true : false;
      }

      inline void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
      {
         g_CurrentFrameKeyStates[static_cast<uint8_t>(mapping::g_GlfwInputKeyMap[1 + button + (uint8_t)(input::InputKey::LastKeyboardKey)])] = (action != GLFW_RELEASE) ? true : false;
      }
         
      inline void Update()
      {
         g_ChangedKeys  = g_CurrentFrameKeyStates ^ g_LastFrameKeyStates;

         g_PressedKeys  = g_ChangedKeys & g_CurrentFrameKeyStates;
         g_ReleasedKeys = g_ChangedKeys & ~g_CurrentFrameKeyStates;

         g_LastFrameKeyStates = g_CurrentFrameKeyStates;
      }


      //
      //All function below don't have any checks for argument validity for purpose
      //They shouldn't be used without wrapper
      //


      inline const bool IsKeyPressOccured(const uint8_t keyId) 
      {
         return g_PressedKeys.test(keyId);
      }

      inline const bool IsKeyReleaseOccured(const uint8_t keyId)
      {
         return g_ReleasedKeys.test(keyId);
      }

      inline const bool IsKeyPressed(const uint8_t keyId)
      {
         return g_CurrentFrameKeyStates.test(keyId);
      }
   }

   inline void SetWindowFocus(bgl::WindowGL& window)
   {
      native::g_FocusedWindow = &window;

      glfwSetKeyCallback(native::g_FocusedWindow->NativeHandle.get(), native::GlfwKeyCallback);
      glfwSetMouseButtonCallback(native::g_FocusedWindow->NativeHandle.get(), native::GlfwMouseButtonCallback);
   }
}