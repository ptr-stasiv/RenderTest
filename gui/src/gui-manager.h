#pragma once
#include <fstream>

#include "render/gui-render.h"
#include "window/window.h"
#include "graphics/api/devices/graphics-device.h"
#include "window/input/input-manager.h"

#include "utils/json.h"
#include "utils/xml.h"
#include "utils/net/http-handle.h"

namespace gui
{
   class GuiManager
   {
   private:
      std::unique_ptr<gui::GuiController> GuiController;

      std::shared_ptr<graphics::VertexBuffer> SurfaceVBO;
      std::shared_ptr<graphics::ShaderProgram> SurfaceShader;
      std::shared_ptr<graphics::Texture2D> SurfaceTexture;

      std::shared_ptr<graphics::GraphicsDevice> GraphicsDevice;


      net::HttpHandle ClientHandle;

      utils::Xml XmlRequests[3]; //0 - new elements, 1 - modified, 2 - deleted
   public:
      GuiManager(const std::shared_ptr<app::Window>& window,
                 const std::shared_ptr<graphics::GraphicsDevice>& gd);

      void Update();

      inline void AddNewElementXML(const utils::XmlTag& tagSource)
      {
         XmlRequests[0].Add(tagSource);
      }

      inline void AddModifyElementXML(const utils::XmlTag& tagSource)
      {
         XmlRequests[1].Add(tagSource);
      }

      inline void AddRemoveElementXML(const utils::XmlTag& tagSource)
      {
         XmlRequests[2].Add(tagSource);
      }

   private:
      void SetupInput(const std::shared_ptr<app::Window>& window);
      void UpdateXmlRequests();
   };
}
