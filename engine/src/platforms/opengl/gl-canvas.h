#pragma once
#include <string_view>
#include <memory>

#include "graphics/api/canvas.h"
#include "events/subject.h"
#include "events/input-events.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace graphics
{
   namespace gl
   {
      namespace callback
      {
         inline event::Subject KeySubject;
         inline event::Subject MouseButtonSubject;
         inline event::Subject CursorSubject;
         inline event::Subject MouseScrollSubject;
         inline event::Subject ResizeSubject;
      }

      class CanvasGL : public Canvas
      {
      private:
         void ImGuiBeginFrame();
         void ImGuiEndFrame();
      public:
         GLFWwindow* GlfwWindow;

         uint16_t Width;
         uint16_t Height;

         CanvasGL(const uint16_t width, const uint16_t height, const std::string_view& title);

         inline ~CanvasGL() override
         {
            glfwDestroyWindow(GlfwWindow);
         }
         
         CanvasGL(CanvasGL&&) = default;
         CanvasGL& operator = (CanvasGL&&) = default;

         inline void BeginFrame() override
         {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glfwPollEvents();

            ImGuiBeginFrame();
         }

         inline void EndFrame() override
         {
            ImGuiEndFrame();

            glfwSwapBuffers(GlfwWindow);
         }

         inline bool ShouldClose() const override
         {
            return glfwWindowShouldClose(GlfwWindow);
         }

         inline void ShowCursor(const bool show) override
         {
            if(show)
               glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
               glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         }

         inline uint16_t GetWidth() const override
         {
            return Width;
         }

         inline uint16_t GetHeight() const override
         {
            return Height;
         }

         inline void AddKeyCallback(const event::Callback& callback) override
         {
            callback::KeySubject.AddObserver(callback);
         }

         inline void AddMouseButtonCallback(const event::Callback& callback) override
         {
            callback::MouseButtonSubject.AddObserver(callback);
         }

         inline void AddCursorCallback(const event::Callback& callback) override
         {
            callback::CursorSubject.AddObserver(callback);
         }

         inline void AddScrollCallback(const event::Callback& callback) override
         {
            callback::MouseScrollSubject.AddObserver(callback);
         }

         inline void AddResizeCallback(const event::Callback& callback) override
         {
            callback::ResizeSubject.AddObserver(callback);
         }

         CanvasGL(const CanvasGL&) = delete;
         CanvasGL& operator = (const CanvasGL&) = delete;
      };
   }
}
