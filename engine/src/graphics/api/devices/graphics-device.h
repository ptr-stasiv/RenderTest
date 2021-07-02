#pragma once
#include <memory>

#include "graphics/api/canvas.h"
#include "graphics/api/shader-program.h"
#include "graphics/api/compute-shader.h"
#include "graphics/api/texture2d.h"
#include "graphics/api/vertex-buffer.h"
#include "graphics/api/uniform-buffer.h"
#include "graphics/api/shader-buffer.h"
#include "graphics/api/framebuffer.h"

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

      virtual std::shared_ptr<ComputeShader> CreateComputeShader() = 0;

      virtual std::shared_ptr<Texture2D> CreateTexture2D() = 0;

      virtual std::shared_ptr<VertexBuffer> CreateVBO() = 0;

      virtual std::shared_ptr<UniformBuffer> CreateUBO() = 0;

      virtual std::shared_ptr<ShaderBuffer> CreateSBO() = 0;

      virtual std::shared_ptr<Framebuffer> CreateFBO() = 0;
      
      
      //Commands
    
      virtual std::string GetDeviceInfo() const = 0;

      virtual void EnableFeature(const Feature feature) const = 0;

      virtual void DisableFeature(const Feature feature) const = 0;

      virtual void Clear() = 0;

      virtual void SetClearColor(const mm::vec4& color) const = 0;

      virtual void SetBlendSettings(const BlendFunc func, const BlendValue src, const BlendValue dst) const = 0;

      virtual void SetCullingFace(const Face face) const = 0;

      virtual void DrawTriangles(const std::shared_ptr<ShaderProgram>& program, const size_t verticesCount) const = 0;
   };
}
