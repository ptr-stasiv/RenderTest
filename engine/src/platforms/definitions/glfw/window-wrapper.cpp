#include "platforms/declarations/window/window-wrapper.h"

#include <Windows.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "debug/gassert.h"
#include "bgl/debug/callback.h"

#include "input/native-input-handling.h"

namespace platform
{
   namespace app
   {
      struct Window::NativeInfo
      {
         inline static GLFWwindow* GlfwWindow;

         inline ~NativeInfo()
         {
            glfwDestroyWindow(GlfwWindow);
         }
      };

      template<typename T>
      T* Window::GetNative() const
      {
         return NativeInfo::GlfwWindow;
      }

      template typename GLFWwindow* Window::GetNative<GLFWwindow>() const;


      Window::Window(const uint16_t width, const uint16_t height, const std::string_view title)
      {
         GASSERT(glfwInit(), "Error in window creation");

         GLFWwindow* window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

         GASSERT(window, "Error in window creation");

         glfwMakeContextCurrent(window);

         GASSERT(glewInit() == GLEW_OK, "Error in window creation");

         //
         //This should be move out from here
         //

         const GLubyte* vendorInfo = glGetString(GL_VENDOR);
         const GLubyte* rendererInfo = glGetString(GL_RENDERER);
         const GLubyte* versionInfo = glGetString(GL_VERSION);
         const GLubyte* glslInfo = glGetString(GL_SHADING_LANGUAGE_VERSION);

         WD_LOG_MESSAGE("%s %s", vendorInfo, rendererInfo);
         WD_LOG_MESSAGE("Opengl %s", versionInfo);
         WD_LOG_MESSAGE("GLSL %s\n", glslInfo);

         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glBlendEquation(GL_FUNC_ADD);

         glEnable(GL_DEPTH_TEST);

         glEnable(GL_DEBUG_OUTPUT);
         glDebugMessageCallback(MessageCallbackOGL, 0);

         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

         Width = width;
         Height = height;
         Title = title;

         NativeInfo::GlfwWindow = window;
      }

      Window::~Window() = default;

      void Window::BeginFrame() const
      {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glfwPollEvents();
      }

      void Window::EndFrame() const
      {
         glfwSwapBuffers(GetNative<GLFWwindow>());
      }

      bool Window::ShouldClose() const
      {
         return glfwWindowShouldClose(GetNative<GLFWwindow>());
      }
   }
}