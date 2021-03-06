#include "gui-wrapper.h"

namespace gui
{
   GuiWrapper::GuiWrapper(const uint32_t width, const uint32_t height)
   {
      GuiController = std::make_unique<gui::GuiController>();

      GuiController->Setup(width, height);

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
               Color = texture(Texture, Uv);
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

      Vbo = gl::CreateVertexBuffer(sizeof(vertices), vertices);

      Vao = gl::CreateVertexArray();

      GLuint b = gl::AddBufferVertexArray(Vao, Vbo, 4 * sizeof(float));

      gl::AddAttribFormatVertexArray(Vao, 0, b, 2, GL_FLOAT, GL_FALSE, 0);
      gl::AddAttribFormatVertexArray(Vao, 1, b, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float));

      gl::TextureParams params;
      params.WrapS = GL_CLAMP_TO_EDGE;
      params.WrapT = GL_CLAMP_TO_EDGE;
      params.MinFilter = GL_NEAREST;
      params.MagFilter = GL_NEAREST;

      SurfaceTexture = gl::CreateTexture(width, height, GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE, params);
   }

   GuiWrapper::~GuiWrapper()
   {
      gl::DeleteVertexArray(Vao); 
      gl::DeleteVertexBuffer(Vbo);
      gl::DeleteTexture2D(SurfaceTexture);
   }

   void GuiWrapper::SetupInput(input::InputManager& inputManager)
   {
      uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(GuiController.get());

      inputManager.GetInputWrapper()->MouseCursorPosEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
         {
            auto mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
            gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

            gc->OnMouseMove(mouseE.PosX, mouseE.PosY);
         }, callbackArgs);

      inputManager.GetInputWrapper()->MouseScrollEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
         {
            auto mouseE = event::CastEvent<event::MouseScrollEvent>(e);
            gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

            gc->OnMouseScroll(mouseE.Value);
         }, callbackArgs);

      inputManager.GetInputWrapper()->MouseButtonEventSubj.AddObserver([](event::BaseEvent& e, uintptr_t args)
         {
            event::MouseButtonEvent mouseE = event::CastEvent<event::MouseButtonEvent>(e);
            gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

            gc->OnMouseButton(mouseE.Button, mouseE.State);
         }, callbackArgs);
   }

   void GuiWrapper::Update()
   {
      GuiShader->Use();

      gl::BindTexture2D(SurfaceTexture, 0);

      uint32_t w, h;
      void* pixels;
      GuiController->GetRenderingInfo(w, h, pixels);

      gl::UpdateTexture2D(SurfaceTexture, w, h, pixels);

      GuiShader->SetInt("Texture", 0);

      glBindVertexArray(Vao.BindId);
      glDrawArrays(GL_TRIANGLES, 0, 12);
      glBindVertexArray(0);
   }
}