#include "gui-manager.h"

#include "utils/read-from-file.h"

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

      const char* vertexShaderSrc = utils::ReadFromFile("res/shaders/gui-surface.vs").data();

      const char* fragmentShaderSrc = utils::ReadFromFile("res/shaders/gui-surface.fs").data();

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