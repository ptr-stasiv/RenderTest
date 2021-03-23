#include "gui-manager.h"

namespace gui
{
   GuiManager::GuiManager(const std::shared_ptr<app::Window>& window,
                          const std::shared_ptr<graphics::GraphicsDevice>& gd)
      : GraphicsDevice(gd)
   {
      uint16_t width = window->GetCanvas()->GetWidth();
      uint16_t height = window->GetCanvas()->GetHeight();

      GuiController = std::make_unique<gui::GuiController>();

      GuiController->Setup(width, height);

      const char* vertexShaderSrc = R"(
            #version 460 core
      
            layout(location = 0) in vec2 pos;
      
            out vec2 Uv;
         
            void main()
            {
               //Mirror coordinates by x axis
               Uv = vec2(pos.x, -pos.y) * 0.5f + 0.5f;
               gl_Position = vec4(pos, 0.0f, 1.0f);
            })";

      const char* fragmentShaderSrc = R"(
            #version 460 core

            #extension GL_ARB_bindless_texture: enable
         
            out vec4 Color;
         
            in vec2 Uv;
      
            layout(bindless_sampler) uniform sampler2D Texture;
      
            void main()
            {
               Color = texture(Texture, Uv);
            })";

      SurfaceShader = GraphicsDevice->CreateShaderProgram();

      SurfaceShader->AddShader(graphics::ShaderType::Vertex, vertexShaderSrc);
      SurfaceShader->AddShader(graphics::ShaderType::Fragment, fragmentShaderSrc);

      SurfaceShader->Compile();


      graphics::TextureParams params;
      params.MagFilter = graphics::TextureFilter::Linear;
      params.MinFilter = graphics::TextureFilter::Nearest;
      params.WrapS = graphics::TextureWrap::ClampToEdge;
      params.WrapT = graphics::TextureWrap::ClampToEdge;

      SurfaceTexture = GraphicsDevice->CreateTexture2D();
      SurfaceTexture->InitData(width, height, 
                               graphics::InternalFormat::RGBA8, graphics::Format::BGRA, graphics::Type::Ubyte,
                               params);

      float vertices[] =
      {
         -1.0f, 1.0f,
         -1.0f, -1.0f,
         1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, -1.0f,
         -1.0f, -1.0f
      };

      SurfaceVBO = GraphicsDevice->CreateVBO();
      SurfaceVBO->InitData(sizeof(vertices), vertices);

      SurfaceShader->AddInputBuffer(SurfaceVBO, 2, 0, sizeof(float) * 2, graphics::Type::Float);

      SetupInput(window);
   }

   void GuiManager::SetupInput(const std::shared_ptr<app::Window>& window)
   {
      uintptr_t callbackArgs = reinterpret_cast<uintptr_t>(GuiController.get());

      event::Callback mouseButtonCallback = { [](event::BaseEvent& e, uintptr_t args)
      {
         event::MouseButtonEvent mouseE = event::CastEvent<event::MouseButtonEvent>(e);
         gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

         //gc->OnMouseButton(mouseE.Button, mouseE.State);
      }, callbackArgs };

      event::Callback cursorCallback = { [](event::BaseEvent& e, uintptr_t args)
      {
         auto mouseE = event::CastEvent<event::MouseCursorPosEvent>(e);
         gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

         gc->OnMouseMove(mouseE.PosX, mouseE.PosY);
      }, callbackArgs };

      event::Callback scrollCallback{ [](event::BaseEvent& e, uintptr_t args)
      {
         auto mouseE = event::CastEvent<event::MouseScrollEvent>(e);
         gui::GuiController* gc = reinterpret_cast<gui::GuiController*>(args);

         gc->OnMouseScroll(mouseE.Value);
      }, callbackArgs };

      window->GetCanvas()->AddMouseButtonCallback(mouseButtonCallback);
      window->GetCanvas()->AddCursorCallback(cursorCallback);
      window->GetCanvas()->AddScrollCallback(scrollCallback);
   }

   void GuiManager::Update()
   {
      SurfaceShader->Use();

      SurfaceShader->SetTexture2D("Texture", SurfaceTexture);

      uint32_t w, h;
      void* pixels;
      GuiController->GetRenderingInfo(w, h, pixels);

      SurfaceTexture->UpdateData(w, h, pixels);
      
      GraphicsDevice->DrawTriangles(SurfaceShader, 12);
   }
}