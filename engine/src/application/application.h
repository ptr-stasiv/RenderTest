#pragma once
#include <memory>

#include "platforms/declarations/window/window-wrapper.h"
#include "platforms/declarations/utils/timer.h"
#include "jobs/job-system.h"
#include "input/input-manager.h"
#include "bgl/buffers/vertex-array.h"
#include "gui.h"

namespace app
{
   class BaseApplication
   {
   private:
      std::shared_ptr<gui::GuiController> GuiController;

      std::unique_ptr<graphics::ShaderPipeline> GuiShader;
      GLuint SurfaceTexture;
      std::unique_ptr<bgl::VertexArray> Vao;
   public:
      std::shared_ptr<platform::app::Window> Window;
      std::shared_ptr<input::InputManager> InputManager;

      float DeltaTime;

      inline BaseApplication()
      {
         Window = std::make_shared<platform::app::Window>();
         InputManager = std::make_shared<input::InputManager>(new input::native::NativeInput(new platform::input::InputWrapper(Window.get())));

         SetupGUI();
      }

      virtual ~BaseApplication() {}

      virtual void OnStartup() {}
      virtual void OnTick() {}

      void SetupGUI()
      {
         GuiController = std::make_shared<gui::GuiController>();
            
         GuiController->Setup(Window->GetWidth(), Window->GetHeight());

         const char* vertexShaderSrc = R"(
            #version 330 core
      
            layout(location = 0) in vec2 pos;
            layout(location = 1) in vec2 uv;
      
            out vec2 Uv;
         
            void main()
            {
               Uv = uv;
               gl_Position = vec4(pos, 0.0f, 1.0f);
            })";

         const char* fragmentShaderSrc = R"(
            #version 330 core
         
            out vec4 Color;
         
            in vec2 Uv;
      
            uniform sampler2D Texture;
      
            void main()
            {
               //Color = texture(Texture, Uv);
               Color=vec4(Uv, 0.0f, 1.0f);
            })";

         GuiShader = std::make_unique<graphics::ShaderPipeline>();

         GuiShader->Add(graphics::ShaderType::Vertex, vertexShaderSrc);
         GuiShader->Add(graphics::ShaderType::Fragment, fragmentShaderSrc);

         GuiShader->Compile();

         float vertices[] =
         {
            -1.0f, -1.0f, 0.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 1.0f,
         };

         bgl::Buffer vbo(sizeof(vertices), vertices);

         Vao = std::make_unique<bgl::VertexArray>();

         GLuint b = Vao->LinkBuffer(vbo, 4 * sizeof(float));

         Vao->AddAttribFormat(0, b, 2, GL_FLOAT, GL_FALSE, 0);
         Vao->AddAttribFormat(1, b, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

         glCreateTextures(GL_TEXTURE_2D, 1, &SurfaceTexture);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, SurfaceTexture);
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Window->GetWidth(), Window->GetHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

         uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(GuiController.get());

         InputManager->GetInputWrapper()->MouseCursorPosEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
            {
               auto mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
               gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

               gc->OnMouseMove(mouseE.PosX, mouseE.PosY);
            }, callbackArgs);

         InputManager->GetInputWrapper()->MouseScrollEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
            {
               auto mouseE = event::CastEvent<event::MouseScrollEvent>(e);
               gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

               gc->OnMouseScroll(mouseE.Value);
            }, callbackArgs);

         InputManager->GetInputWrapper()->MouseButtonEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
            {
               event::MouseButtonEvent mouseE = event::CastEvent<event::MouseButtonEvent>(e);
               gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

               gc->OnMouseButton(mouseE.Button, mouseE.State);
            }, callbackArgs);
      }

      void GuiUpdate()
      {
         GuiShader->Use();

         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, SurfaceTexture);

         uint32_t w, h;
         void* pixels;
         GuiController->GetRenderingInfo(w, h, pixels);

         glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

         GuiShader->SetInt("Texture", 0);

         glBindVertexArray(Vao->BindId);
         glDrawArrays(GL_TRIANGLES, 0, 12);
         glBindVertexArray(0);
      }
   };

   extern BaseApplication* CreateApplication();
}

int main()
{
   auto app = std::shared_ptr<app::BaseApplication>(app::CreateApplication());

   core::JobSystem::Setup();

   app->OnStartup();

   platform::utils::Timer deltaTimer;

   while (!app->Window->ShouldClose())
   {
      deltaTimer.Reset();

      app->Window->BeginFrame();
      app->InputManager->Poll();

      app->OnTick();
      app->GuiUpdate();

      app->Window->EndFrame();

      app->DeltaTime = 1.0f / deltaTimer.GetElaspedTime();
   }

   return 0;
}