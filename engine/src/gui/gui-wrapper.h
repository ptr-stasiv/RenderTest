#pragma once
#include "gui.h"
#include "graphics/shaders/shader-pipeline.h"
#include "platforms/opengl/primitives/buffers/vertex-array.h"
#include "platforms/opengl/primitives/textures/texture2d.h"
#include "input/input-manager.h"

namespace gui
{
   class GuiWrapper
   {
   private:
      std::unique_ptr<gui::GuiController> GuiController;
      std::unique_ptr<graphics::ShaderPipeline> GuiShader;

      gl::VertexArray  Vao;
      gl::VertexBuffer Vbo;
      gl::Texture2D SurfaceTexture;
   public:
      GuiWrapper(const uint32_t width, const uint32_t height);
      ~GuiWrapper();

      void SetupInput(input::InputManager& inputManager);

      void Update();
   };
}