#pragma once
#include "GLFW/glfw3.h"
#include "math/vectors/vector2.h"

namespace input
{
   namespace native
   {
      extern GLFWwindow* g_FocusedWindow;


      using KeyFunc = void(*)(uint32_t, uint32_t, uint32_t, uint32_t);
      using MouseButtonFunc = void(*)(uint32_t, uint32_t, uint32_t);

      struct Callbacks
      {
         static inline KeyFunc         KeyCallbackFunc;
         static inline MouseButtonFunc MouseButtonCallbackFunc;
      };

      inline void GlfwKeyCallback(GLFWwindow* window, int key, int sc, int action, int mods)
      {
         Callbacks::KeyCallbackFunc(key, sc, action, mods);
      }

      inline void GlfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
      {
         Callbacks::MouseButtonCallbackFunc(button, action, mods);
      }


      extern float g_ScrollValue;

      inline void GlfwMouseScrollCallback(GLFWwindow* window, double x, double y)
      {
         g_ScrollValue = y;
      }

      inline math::Vector2 GetCursorPosition()
      {
         double x, y;
         glfwGetCursorPos(g_FocusedWindow, &x, &y);

         return { float(x), float(y) };
      }

      inline float GetScrollValue()
      {
         return g_ScrollValue;
      }


      inline void SetWindowFocus(GLFWwindow* window)
      {
         g_FocusedWindow = window;

         glfwSetKeyCallback(g_FocusedWindow, native::GlfwKeyCallback);
         glfwSetMouseButtonCallback(g_FocusedWindow, native::GlfwMouseButtonCallback);
         glfwSetScrollCallback(g_FocusedWindow, native::GlfwMouseScrollCallback);
      }
   }
}