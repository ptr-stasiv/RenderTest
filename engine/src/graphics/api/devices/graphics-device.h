#pragma once
#include <memory>

#include "graphics/api/canvas.h"
#include "graphics/api/shader-program.h"
#include "graphics/api/compute-shader.h"
#include "graphics/api/texture2d.h"
#include "graphics/api/vertex-buffer.h"
#include "graphics/api/uniform-buffer.h"
#include "graphics/api/shader-buffer.h"

namespace graphics
{
   class GraphicsDevice
   {
   public:
      GraphicsDevice() = default;

      virtual ~GraphicsDevice() = default;

      virtual std::shared_ptr<Canvas> CreateCanvas(const uint16_t sizeX, const uint16_t sizeY,
                                                   const std::string_view& title) = 0;

      virtual std::shared_ptr<ShaderProgram> CreateShaderProgram() = 0;

      //virtual std::unique_ptr<ComputeShader> CreateComputeShader() = 0;

      virtual std::shared_ptr<Texture2D> CreateTexture2D() = 0;

      virtual std::shared_ptr<VertexBuffer> CreateVBO() = 0;

      //virtual std::unique_ptr<UniformBuffer> CreateUBO() = 0;

      //virtual std::unique_ptr<ShaderBuffer> CreateSBO() = 0;
   };
}
