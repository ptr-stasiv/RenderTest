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

      virtual std::unique_ptr<Canvas> CreateCanvas() = 0;

      virtual std::unique_ptr<ShaderProgram> CreateShaderProgram() = 0;

      //virtual std::unique_ptr<ComputeShader> CreateComputeShader() = 0;

      virtual std::unique_ptr<Texture2D> CreateTexture2D() = 0;

      virtual std::unique_ptr<VertexBuffer> CreateVBO() = 0;

      //virtual std::unique_ptr<UniformBuffer> CreateUBO() = 0;

      //virtual std::unique_ptr<ShaderBuffer> CreateSBO() = 0;
   };
}
