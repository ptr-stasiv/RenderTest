#pragma once
#include "graphics-device.h"

#include "platforms/opengl/gl-api.h"
#include "platforms/opengl/gl-canvas.h"
#include "platforms/opengl/gl-shader.h"
#include "platforms/opengl/gl-texture2d.h"
#include "platforms/opengl/gl-vertex-buffer.h"

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
      
      
      //Commands
      
     inline std::string GetDeviceInfo() const override
     {
         return "";
     }

     inline void EnableFeature(const Feature feature) const override
     {
         glEnable(OGL_FEATURE(feature));
     }

     inline void DisableFeature(const Feature feature) const override
     {
         glDisable(OGL_FEATURE(feature));
     }

     inline void DrawTriangles(const std::shared_ptr<ShaderProgram>& program, const size_t verticesCount) const override
     {
        auto& glProgram = std::static_pointer_cast<gl::ShaderProgramGL>(program);

        glProgram->Use();
        glBindVertexArray(glProgram->Vao);

        glDrawArrays(GL_TRIANGLES, 0, verticesCount);
     }

     inline void Clear() override
     {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     }

     inline void SetClearColor(const math::Vector4& color) const override
     {
         glClearColor(color.x, color.y, color.z, color.w);
     } 

     inline void SetBlendSettings(const BlendFunc func, const BlendValue src, const BlendValue dst) const override
     {
         glBlendEquation(OGL_BLEND_FUNC(func));
         glBlendFunc(OGL_BLEND_VALUE(src), OGL_BLEND_VALUE(dst));
     } 
   };
}
