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

      std::shared_ptr<graphics::ShaderProgram> SurfaceShader;
      std::shared_ptr<graphics::Texture2D> SurfaceTexture;

      event::Subject MouseButtonSubject;
      event::Subject CursorSubject;
      event::Subject ScrollSubject;
   public:
      GuiManager(const std::shared_ptr<app::Window>& window,
                 const std::shared_ptr<input::InputManager>& inputM,
                 const std::shared_ptr<graphics::GraphicsDevice>& gd);

      ~GuiManager();

      void Update();
   private:
      void SetupInput();
   };
}
