#pragma once
#include <cstdint>
#include <bitset>

#include "bgl/window/window-wrapper.h"
#include "glfw/glfw-key-remapping.h"
#include "debug/log/log.h"
#include "math/vectors/vector2.h"

namespace input
{
   namespace native
   {
      static bgl::WindowGL* g_GlfwFocusedWindow;

      //
      //'Event' mean all events that can be done with input devices like keyboard key press or mouse move
      //

      static constexpr uint8_t MaxEvents = (uint8_t)(input::InputEvent::LAST_ENUM_ELEMENT);

      extern std::bitset<MaxEvents> g_CurrentFrameEventStates;
      extern std::bitset<MaxEvents> g_LastFrameEventStates;
      extern std::bitset<MaxEvents> g_ReleasedEvents;
      extern std::bitset<MaxEvents> g_PressedEvents;
      extern std::bitset<MaxEvents> g_ChangedEvents;

      
      //
      //GLFW callbacks
      //

      inline void GlfwKeyCallback(GLFWwindow* window, int key, int sc, int action, int mods)
      {
         g_CurrentFrameEventStates[static_cast<uint8_t>(mapping::g_GlfwInputEventMap[key])] = (action != GLFW_RELEASE) ? true : false;
      }

      inline void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
      {
         g_CurrentFrameEventStates[static_cast<uint8_t>(mapping::g_GlfwInputEventMap[1 + button + (uint8_t)(input::InputEvent::LastKeyboardKey)])] = (action != GLFW_RELEASE) ? true : false;
      }


      static float g_ScrollValue;

      inline void GlfwMouseScrollCallback(GLFWwindow* window, double x, double y)
      {
         g_ScrollValue = y;
      }
       
      //
      //
      //

      inline math::Vector2 GetCursorPosition()
      {
         double x, y;
         glfwGetCursorPos(g_GlfwFocusedWindow->NativeHandle.get(), &x, &y);

         return { float(x), float(y) };
      }

      inline float GetScrollValue()
      {
         return g_ScrollValue;
      }


      void Update();


      //
      //All function below don't have any checks for argument validity for purpose
      //They shouldn't be used without wrapper
      //


      inline const bool IsPressOccured(const uint8_t keyId) 
      {
         return g_PressedEvents.test(keyId);
      }

      inline const bool IsReleaseOccured(const uint8_t keyId)
      {
         return g_ReleasedEvents.test(keyId);
      }

      inline const bool IsPressed(const uint8_t keyId)
      {
         return g_CurrentFrameEventStates.test(keyId);
      }
   }

   inline void SetWindowFocus(bgl::WindowGL& window)
   {
      native::g_GlfwFocusedWindow = &window;

      glfwSetKeyCallback(native::g_GlfwFocusedWindow->NativeHandle.get(), native::GlfwKeyCallback);
      glfwSetMouseButtonCallback(native::g_GlfwFocusedWindow->NativeHandle.get(), native::GlfwMouseButtonCallback);
      glfwSetScrollCallback(native::g_GlfwFocusedWindow->NativeHandle.get(), native::GlfwMouseScrollCallback);
   }
}