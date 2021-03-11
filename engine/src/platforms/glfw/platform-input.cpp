#include "window/input/platform-input.h"

#include <functional>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "debug/gassert.h"


namespace input
{
   static std::vector<event::Subject> KeySubjectList;
   static std::vector<event::Subject> MouseButtonSubjectList;
   static std::vector<event::Subject> CursorSubjectList;
   static std::vector<event::Subject> ScrollSubjectList;

   namespace callback
   {
      inline void KeyCallback(GLFWwindow* w, int key, int sc, int action, int mods)
      {
         for (auto e : KeySubjectList)
            e.Invoke(event::KeyEvent(key, action));
      }

      inline void MouseButtonCallback(GLFWwindow* w, int button, int action, int mods)
      {
         for (auto e : MouseButtonSubjectList)
            e.Invoke(event::KeyEvent(button, action));
      }

      inline void CursorCallback(GLFWwindow* w, double x, double y)
      {
         for (auto e : CursorSubjectList)
            e.Invoke(event::MouseCursorPosEvent(x, y));
      }

      inline void ScrollCallback(GLFWwindow* w, double x, double y)
      {
         for (auto e : ScrollSubjectList)
            e.Invoke(event::MouseScrollEvent(y));
      }
   }

   namespace native
   {
      void SetupInputCallbacks(const std::shared_ptr<app::Window>& window)
      {
         glfwSetKeyCallback(window->GetNative<GLFWwindow>(), callback::KeyCallback);

         glfwSetMouseButtonCallback(window->GetNative<GLFWwindow>(), callback::MouseButtonCallback);

         glfwSetCursorPosCallback(window->GetNative<GLFWwindow>(), callback::CursorCallback);

         glfwSetScrollCallback(window->GetNative<GLFWwindow>(), callback::ScrollCallback);
      }

      void AddKeyCallback(const event::Subject& keySubject)
      {
         KeySubjectList.push_back(keySubject);
      }

      void AddMouseButtonCallback(const event::Subject& mouseSubject)
      {
         MouseButtonSubjectList.push_back(mouseSubject);
      }

      void AddCursorCallback(const event::Subject& cursorSubject)
      {
         CursorSubjectList.push_back(cursorSubject);
      }

      void AddScrollCallback(const event::Subject& scrollCallback)
      {
         ScrollSubjectList.push_back(scrollCallback);
      }
   }
}