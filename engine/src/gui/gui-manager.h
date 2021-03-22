#pragma once
#include "gui.h"
#include "window/window.h"
#include "graphics/api/devices/graphics-device.h"
#include "window/input/input-manager.h"

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
   public:
      GuiManager(const std::shared_ptr<app::Window>& window,
                 const std::shared_ptr<graphics::GraphicsDevice>& gd);

      void Update();
   private:
      void SetupInput(const std::shared_ptr<app::Window>& window);
   };
}
