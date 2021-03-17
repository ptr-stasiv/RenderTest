#pragma once
#include "graphics-device.h"

#include "platforms/opengl/gl-canvas.h"
#include "platforms/opengl/gl-shader.h"
#include "platforms/opengl/gl-texture2d.h"
#include "platforms/opengl/gl-vertex-buffer.h"
#include "platforms/opengl/gl-canvas.h"

namespace graphics
{
   class OpenglGraphicsDevice : public GraphicsDevice
   {
   public:
      OpenglGraphicsDevice() = default;
      ~OpenglGraphicsDevice() = default;

      inline std::shared_ptr<Canvas> CreateCanvas(const uint16_t sizeX, const uint16_t sizeY,
                                                  const std::string_view& title) override
      {
         return std::make_shared<gl::CanvasGL>(sizeX, sizeY, title); 
      }

      inline std::shared_ptr<ShaderProgram> CreateShaderProgram() override
      {
         return std::make_shared<gl::ShaderProgramGL>();
      }

      //std::unique_ptr<ComputeShader> CreateComputeShader() override;

      inline std::shared_ptr<Texture2D> CreateTexture2D() override
      {
         return std::make_shared<gl::Texture2dGL>();
      }

      inline std::shared_ptr<VertexBuffer> CreateVBO() override
      {
         return std::make_shared<gl::VertexBufferGL>();
      }

      //std::unique_ptr<UniformBuffer> CreateUBO() override;

      //std::unique_ptr<ShaderBuffer> CreateSBO() override;
   };
}
