#include "gui-manager.h"

#include "utils/read-from-file.h"
#include "platforms/win64/win64-dev.h"

std::string GetTempDir()
#ifdef WIN64
   {
      char buf[256];
      GetTempPathA(sizeof(buf), buf);
      return buf;
   }
#endif

//On other platforms(other I mean not Windows) can be issues with slash direction
const std::string XmlFilesDirs[3] = 
{
   GetTempDir() + "newFile.xml",
   GetTempDir() + "modFile.xml",
   GetTempDir() + "remFile.xml"
};

const char* ServerEvents[3] = 
{
   "Expand",
   "Modify",
   "Remove"
};

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

      
      ClientHandle = net::InitializeClientHTTP("127.0.0.1", 3333);

      for(size_t i = 0; i < 3; ++i)
         XmlFiles[i].open(XmlFilesDirs[i], std::ofstream::trunc);

      utils::XmlTag tag("log", { { "id", "123"} }, "");
      AddNewElementXML(tag);
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

   void GuiManager::UpdateXmlRequests()
   {
     for(size_t i = 0; i < 3; ++i)
     {
         if(XmlRequests[i].Empty())
            continue;
         
         auto data = XmlRequests[i].ToString();

         XmlFiles[i].write(data.c_str(), data.length());

         utils::Json json;
         json["event"] = ServerEvents[i];
         json["params"] = XmlFilesDirs[i].c_str(); //TODO fix string assigment

         auto jsonStr = json.ToString();


         net::HttpRequest req;
         req.Method = "POST";
         req.ContentType = "application/json";
         req.ContentSize = jsonStr.length();

         net::SendRequestHTTP(ClientHandle, req, jsonStr);

         XmlRequests[i].Clear();
     }
   }

   void GuiManager::Update()
   {
      UpdateXmlRequests();

      SurfaceShader->Use();

      SurfaceShader->SetTexture2D("Texture", SurfaceTexture);

      uint32_t w, h;
      void* pixels;
      GuiController->GetRenderingInfo(w, h, pixels);

      SurfaceTexture->UpdateData(w, h, pixels);
      
      GraphicsDevice->DrawTriangles(SurfaceShader, 12);
   }
}
