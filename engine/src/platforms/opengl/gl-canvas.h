#pragma once
#include <string_view>
#include <memory>

#include "graphics/api/canvas.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace graphics
{
   namespace gl
   {
      class CanvasGL : public Canvas
      {
      private:
         uint16_t Width;
         uint16_t Height;

         struct NativeInfo
         {
            GLFWwindow* GlfwWindow;

            inline ~NativeInfo()
            {
               glfwDestroyWindow(GlfwWindow);
            }
         } Native;
      public:
         CanvasGL(const uint16_t width, const uint16_t height, const std::string_view& title);
         ~CanvasGL() override = default;
         
         CanvasGL(CanvasGL&&) = default;
         CanvasGL& operator = (CanvasGL&&) = default;

         inline void BeginFrame() override
         {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwPollEvents();
         }

         inline void EndFrame() override
         {
            glfwSwapBuffers(Native.GlfwWindow);
         }

         bool ShouldClose() override
         {
            return glfwWindowShouldClose(Native.GlfwWindow);
         }

         inline std::any GetNative() override
         {
            return &Native;
         }

         inline uint16_t GetWidth() const override
         {
            return Width;
         }

         inline uint16_t GetHeight() const override
         {
            return Height;
         }

         CanvasGL(const CanvasGL&) = delete;
         CanvasGL& operator = (const CanvasGL&) = delete;
      };
   }
}
