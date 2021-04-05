#pragma once
#include <memory>

#include "asset-manager/asset-manager.h"
#include "math/vectors/vector.h"
#include "math/matrices/matrix4x4.h"

#include "graphics/api/shader-program.h"
#include "graphics/api/texture2d.h"
#include "graphics/api/devices/graphics-device.h"
#include "utils/read-from-file.h"
#include "debug/globals.h"

namespace graphics
{
   class BaseMaterial;

   struct Mesh
   {
      assets::TrigVertices Vertices;

      std::unique_ptr<BaseMaterial> Material;

      math::Vector3 Scale;
      math::Vector3 Rotation; //This later will be replaced
      math::Vector3 Translate;
   };

   class BaseMaterial
   {
   protected:
      std::string VertexShaderPath = "";
      std::string FragmentShaderPath = "";
   private: 
      std::shared_ptr<ShaderProgram> ShaderProgram;      
   public:
      inline BaseMaterial(const std::shared_ptr<graphics::GraphicsDevice>& gd)
      {
         std::string_view vertexShaderSrc;
         std::string_view fragmentShaderSrc;

         if((vertexShaderSrc = utils::ReadFromFile(VertexShaderPath)) == "")
            PRINT_AND_TERMINATE("Couldn't read vertex shader file: %s", VertexShaderPath.c_str());
         if((fragmentShaderSrc = utils::ReadFromFile(FragmentShaderPath)) == "")
            PRINT_AND_TERMINATE("Couldn't read fragment shader file: %s", FragmentShaderPath.c_str());

         ShaderProgram = gd->CreateShaderProgram();

         ShaderProgram->AddShader(ShaderType::Vertex, vertexShaderSrc);
         ShaderProgram->AddShader(ShaderType::Fragment, fragmentShaderSrc);

         ShaderProgram->Compile();
      }

      virtual ~BaseMaterial() = default;

      virtual void ResolveUniforms() {}
   };

   class PhongMaterial : public BaseMaterial
   {
   protected:
      std::string VertexShaderPath = "res/shaders/phong-shading.vs";
      std::string FragmentShaderPath = "res/shaders/phong-shading.vs";
   public:
      inline PhongMaterial(const std::shared_ptr<GraphicsDevice>& gd)
         : BaseMaterial(gd) {}
   public:
      math::Vector4 Diffuse;

      math::Vector3 Specular;

      math::Vector3 Emissive;

      float Glossiness;

      assets::PixelsData DiffuseTexture;
      assets::PixelsData SpecularTexture;
      assets::PixelsData NormalTexture;
      assets::PixelsData EmissiveTexture;
      assets::PixelsData GlossinessTexture;


      virtual void ResolveUniforms() override
      {

      }
   }; 
}
