#include "gl-canvas.h"

#include "debug/globals.h"
#include "glfw/glfw-input-map.h"
#include "events/input-events.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace graphics
{
   namespace gl
   { 
      namespace callback
      {
         inline void KeyCallback(GLFWwindow* w, int key, int sc, int action, int mods)
         {
            auto f = GlfwKeyMap.find(key);
            if (f == GlfwKeyMap.end())
            {
               LOG_ERROR("Unprocessed key is pressed!");
               return;
            }

            KeySubject.Invoke(event::KeyEvent(f->second, GlfwTranslateState(action)));
         }

         inline void MouseButtonCallback(GLFWwindow* w, int button, int action, int mods)
         {
            auto f = GlfwMouseButtonMap.find(button);
            if (f == GlfwMouseButtonMap.end())
            {
               LOG_ERROR("Unprocessed mouse button is pressed!");
               return;
            }

            MouseButtonSubject.Invoke(event::MouseButtonEvent(f->second, GlfwTranslateState(action)));
         }

         inline void CursorCallback(GLFWwindow* w, double x, double y)
         {
            CursorSubject.Invoke(event::MouseCursorPosEvent(x, y));
         }

         inline void ScrollCallback(GLFWwindow* w, double x, double y)
         {
            MouseScrollSubject.Invoke(event::MouseScrollEvent(y));
         }

         inline void ResizeCallback(GLFWwindow* w, int width, int height)
         {
            ResizeSubject.Invoke(event::ResizeEvent(width, height));
         }
      }

      const char* g_MessageSources[] =
      {
         "OpenGL",
         "Window System",
         "Shader compiler",
         "Third party",
         "Application",
         "Other"
      };

      const char* g_MessageType[] =
      {
         "Error",
         "Deprecated",
         "Undefined",
         "Portability",
         "Performance",
         "Marker",
         "Group",
         "Group",
         "Other"
      };

      void GLAPIENTRY ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
         GLsizei length, const GLchar* message, const void* userParam)
      {
         const char* messageSource = g_MessageSources[source - GL_DEBUG_SOURCE_API];
         const char* messageType = g_MessageType[type - GL_DEBUG_TYPE_ERROR];

         switch (severity)
         {
         case GL_DEBUG_SEVERITY_HIGH:
         {
            WD_LOG_ERROR("[%s][%s] %s", messageSource, messageType, message);
         }break;
         case GL_DEBUG_SEVERITY_MEDIUM:
         {
            WD_LOG_WARNING("[%s][%s] %s", messageSource, messageType, message);
         }break;
         case GL_DEBUG_SEVERITY_LOW:
         {
            WD_LOG_WARNING("[%s][%s] %s", messageSource, messageType, message);
         }break;
         //case GL_DEBUG_SEVERITY_NOTIFICATION:
         //{
         //   WD_LOG_MESSAGE("[%s][%s] %s", messageSource, messageType, message);
         //}break;
         default:
            break;
         }
      }

      CanvasGL::CanvasGL(const uint16_t width, const uint16_t height, const std::string_view& title)
         : Width(width), Height(height)
      {
         if (!glfwInit())
            PRINT_AND_TERMINATE("Error in OpengGL initialization!");

         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

         GlfwWindow = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

         if (!GlfwWindow)
            PRINT_AND_TERMINATE("Error in OpenGL window creation!");

         glfwMakeContextCurrent(GlfwWindow);

         if(glewInit() != GLEW_OK)
            PRINT_AND_TERMINATE("Error in OpengGL initialization!");


         //Alpha blending
         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         glBlendEquation(GL_FUNC_ADD);

         glEnable(GL_DEPTH_TEST);

         glEnable(GL_CULL_FACE);

         glEnable(GL_DEBUG_OUTPUT);
         //glDebugMessageCallback(ErrorCallback, 0);


         //Input setup

         glfwSetKeyCallback(GlfwWindow, callback::KeyCallback);

         glfwSetMouseButtonCallback(GlfwWindow, callback::MouseButtonCallback);

         glfwSetCursorPosCallback(GlfwWindow, callback::CursorCallback);

         glfwSetScrollCallback(GlfwWindow, callback::ScrollCallback);

         glfwSetWindowSizeCallback(GlfwWindow, callback::ResizeCallback);

         glfwSetInputMode(GlfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



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


         ImGui::CreateContext();
         ImGuiIO& io = ImGui::GetIO(); (void)io;
         io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
         io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

         ImGui::StyleColorsDark();

         ImGuiStyle& style = ImGui::GetStyle();
         if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
         {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
         }

         ImGui_ImplGlfw_InitForOpenGL(GlfwWindow, true);
         ImGui_ImplOpenGL3_Init("#version 130");
      }

      void CanvasGL::ImGuiBeginFrame()
      {
         ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplGlfw_NewFrame();
         ImGui::NewFrame();
      }

      void CanvasGL::ImGuiEndFrame()
      {
         ImGui::Render();
         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
         {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
         }
      }
   }
}
