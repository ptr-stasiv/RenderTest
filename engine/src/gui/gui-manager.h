#pragma once
#include "gui.h"
#include "graphics/shaders/shader-program.h"
#include "platforms/opengl/primitives/buffers/vertex-array.h"
#include "platforms/opengl/primitives/textures/texture2d.h"
#include "window/input/input-manager.h"

namespace gui
{
   class GuiManager
   {
   private:
      std::unique_ptr<gui::GuiController> GuiController;
      std::unique_ptr<graphics::ShaderProgram> GuiShader;

      gl::VertexArray  Vao;
      gl::VertexBuffer Vbo;
      gl::Texture2D SurfaceTexture;

      event::Subject MouseButtonSubject;
      event::Subject CursorSubject;
      event::Subject ScrollSubject;
   public:
      GuiManager(const std::shared_ptr<app::Window>& window);

      ~GuiManager();

      void Update();
   private:
      void SetupInput();
   };
}