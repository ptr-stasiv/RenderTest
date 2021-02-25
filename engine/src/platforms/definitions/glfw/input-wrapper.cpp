#include "platforms/declarations/window/input-wrapper.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "debug/gassert.h"

#include <functional>

namespace platform
{
   namespace input
   {
      namespace callback
      {
         inline void KeyCallback(GLFWwindow* w, int key, int sc, int action, int mods)
         {
            InputWrapper* inputH = reinterpret_cast<InputWrapper*>(glfwGetWindowUserPointer(w));
            inputH->KeyEventSubj.Invoke(event::KeyEvent(key, action));
         }

         inline void MouseButtonCallback(GLFWwindow* w, int button, int action, int mods)
         {
            InputWrapper* inputH = reinterpret_cast<InputWrapper*>(glfwGetWindowUserPointer(w));
            inputH->MouseButtonEventSubj.Invoke(event::MouseButtonEvent(button, action));
         }

         inline void CursorCallback(GLFWwindow* w, double x, double y)
         {
            InputWrapper* inputH = reinterpret_cast<InputWrapper*>(glfwGetWindowUserPointer(w));
            inputH->MouseCursorPosEventSubj.Invoke(event::MouseCursorPosEvent(x, y));
         }

         inline void ScrollCallback(GLFWwindow* w, double x, double y)
         {
            InputWrapper* inputH = reinterpret_cast<InputWrapper*>(glfwGetWindowUserPointer(w));
            inputH->MouseScrollEventSubj.Invoke(event::MouseScrollEvent(y));
         }
      }

      InputWrapper::InputWrapper(app::Window* window)
      {
         auto n = window->GetNative<GLFWwindow>();
         glfwSetWindowUserPointer(window->GetNative<GLFWwindow>(), reinterpret_cast<void*>(this));


         glfwSetKeyCallback(window->GetNative<GLFWwindow>(), callback::KeyCallback);

         glfwSetMouseButtonCallback(window->GetNative<GLFWwindow>(), callback::MouseButtonCallback);

         glfwSetCursorPosCallback(window->GetNative<GLFWwindow>(), callback::CursorCallback);

         glfwSetScrollCallback(window->GetNative<GLFWwindow>(), callback::ScrollCallback);
      }
   }
}