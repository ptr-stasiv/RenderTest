#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string_view>
#include <memory>

namespace bgl
{
   class WindowGL
   {
   public:
      uint16_t Width;
      uint16_t Height;
      std::string_view Title;

      std::unique_ptr<GLFWwindow, void(*)(GLFWwindow*)> NativeHandle;

      inline WindowGL() 
         : NativeHandle(nullptr, nullptr) {}

      ~WindowGL() = default;

      bool Instantiate(const uint16_t width = 1280, const uint16_t height = 720,
                       const std::string_view title = "RenderTest");

      inline void BeginFrame() const
      {
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glfwPollEvents();
      }

      inline void EndFrame() const
      {
         glfwSwapBuffers(NativeHandle.get());
      }

      inline bool ShouldClose() const
      {
         return glfwWindowShouldClose(NativeHandle.get());
      }

      inline const uint32_t GetWidth() const
      {
         return Width;
      }

      inline const uint32_t GetHeight() const
      {
         return Height;
      }

      WindowGL(const WindowGL*) = delete;
      WindowGL(WindowGL&&)      = delete;
      WindowGL& operator = (const WindowGL&) = delete;
      WindowGL& operator = (WindowGL&&)      = delete;
   };
}