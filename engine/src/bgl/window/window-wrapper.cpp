#include "window-wrapper.h"
#include "debug/gassert.h"
#include "bgl/debug/callback.h"

namespace bgl
{
   bool WindowGL::Instantiate(const uint16_t width, const uint16_t height,
                              const std::string_view title)
   {
      if (!glfwInit())
         return false;

      GLFWwindow* window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

      if (!window)
         return false;

      glfwMakeContextCurrent(window);

      if (glewInit() != GLEW_OK)
         return false;

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

      Width  = width;
      Height = height;
      Title  = title;
      NativeHandle = std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)>(window, 
                                                                       [](GLFWwindow* w) 
                                                                       { glfwDestroyWindow(w); });
      return true;
   }
}