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

      std::shared_ptr<BaseMaterial> Material;

      math::Vector3 Scale;
      math::Vector3 Rotation; //This later will be replaced
      math::Vector3 Translate;
   };

   inline size_t GenerateMaterialId()
   {
      static size_t instanceId = -1;
      return ++instanceId;
   }

   class BaseMaterial
   {
   protected:
      std::shared_ptr<ShaderProgram> ShaderProgram;    

      size_t InstanceId = -1;
   public:
      inline BaseMaterial(const std::string_view& vertexShaderPath, const std::string_view& fragmentShaderPath, 
                          const std::shared_ptr<graphics::GraphicsDevice>& gd)
         : InstanceId(GenerateMaterialId())
      {
         std::string_view vertexShaderSrc;
         std::string_view fragmentShaderSrc;

         if((vertexShaderSrc = utils::ReadFromFile(vertexShaderPath)) == "")
            PRINT_AND_TERMINATE("Couldn't read vertex shader file: %s", &vertexShaderPath[0]);
         if((fragmentShaderSrc = utils::ReadFromFile(fragmentShaderPath)) == "")
            PRINT_AND_TERMINATE("Couldn't read fragment shader file: %s", &fragmentShaderPath[0]);

         ShaderProgram = gd->CreateShaderProgram();

         ShaderProgram->AddShader(ShaderType::Vertex, vertexShaderSrc);
         ShaderProgram->AddShader(ShaderType::Fragment, fragmentShaderSrc);

         ShaderProgram->Compile();
      }

      virtual ~BaseMaterial() = default;

      inline BaseMaterial(const BaseMaterial& baseMaterial)
         : InstanceId(GenerateMaterialId())
      {
         ShaderProgram = baseMaterial.ShaderProgram;
         LOG_WARNING("Copy constructor");
      }

      inline BaseMaterial& operator = (const BaseMaterial& baseMaterial)
      {
         InstanceId = GenerateMaterialId();

         ShaderProgram = baseMaterial.ShaderProgram;
         LOG_WARNING("Copy operator");
      }

      BaseMaterial(BaseMaterial&&) = default;
      BaseMaterial& operator = (BaseMaterial&&) = default;

      inline void Bind() const
      {
         ShaderProgram->Use();
      }

      inline const size_t GetId() const
      {
         return InstanceId;
      }

      //You shouldn't bind shader in this function
      virtual void ResolveUniforms() {}


      //Through this functions render supply global information to shader
      
      virtual void SetObjectToWorldMatrix(const math::Matrix4& mat) {} 
      virtual void SetWorldToCameraMatrix(const math::Matrix4& mat) {}
      virtual void SetCameraToClipMatrix(const math::Matrix4& mat) {}
      virtual void SetCameraPosition(const math::Vector3& position) {}
      virtual void SetSpotlightsCount(const size_t count) {}
      virtual void SetPointLightsCount(const size_t count) {}


      //This functions are used by the render to get infomation about shader
   
      virtual size_t GetPositionAttrib() const 
      {
         return -1;
      }

      virtual size_t GetNormalAttrib() const
      {
         return -1;
      }

      virtual size_t GetUvAttrib() const
      {
         return -1;
      }

      virtual size_t GetTangentAttrib() const
      {
         return -1;
      }

      virtual std::string_view GetLightUBO() const
      {
         return "";
      }
   };

   class PhongMaterial : public BaseMaterial
   {
   public:
      inline PhongMaterial(const std::shared_ptr<GraphicsDevice>& gd)
         : BaseMaterial("res/shaders/phong-shading.vs", 
                        "res/shaders/phong-shading.fs", gd) {}
      math::Vector4 Diffuse;

      math::Vector3 Specular;

      math::Vector3 Emissive;

      float Glossiness;

      std::shared_ptr<Texture2D>  DiffuseTexture;
      std::shared_ptr<Texture2D>  SpecularTexture;
      std::shared_ptr<Texture2D>  NormalTexture;
      std::shared_ptr<Texture2D>  EmissiveTexture;
      std::shared_ptr<Texture2D>  GlossinessTexture;


      virtual void ResolveUniforms() override
      {
         ShaderProgram->SetFloats("Diffuse", Diffuse);

         ShaderProgram->SetFloats("Specular", Specular);

         ShaderProgram->SetFloats("Emissive", Emissive);

         ShaderProgram->SetFloat("Glossiness", Glossiness);

 

         //TODO rewrite this
         
         if(DiffuseTexture)
         { 
            ShaderProgram->SetInt("DiffuseTextureSupplied", 1);
            ShaderProgram->SetTexture2D("DiffuseTexture", DiffuseTexture);
         }

         if(SpecularTexture)
         {
            ShaderProgram->SetInt("SpecularTextureSupplied", 1);
            ShaderProgram->SetTexture2D("SpecularTexture", SpecularTexture);
         }

         if(NormalTexture)
         {
            ShaderProgram->SetInt("NormalTextureSupplied", 1);
            ShaderProgram->SetTexture2D("NormalTexture", NormalTexture);
         }

         if(EmissiveTexture)
         {
            ShaderProgram->SetInt("EmissiveTextureSupplied", 1);
            ShaderProgram->SetTexture2D("EmissiveTexture", EmissiveTexture);
         }

         if(GlossinessTexture)
         {
            ShaderProgram->SetInt("GlossinessTextureSupplied", 1);
            ShaderProgram->SetTexture2D("GlossinessTexture", GlossinessTexture);
         }
      }

      virtual void SetObjectToWorldMatrix(const math::Matrix4& mat) override 
      {
         ShaderProgram->SetFloats("ToWorld", mat);
      } 

      virtual void SetWorldToCameraMatrix(const math::Matrix4& mat) override 
      {
         ShaderProgram->SetFloats("ToCamera", mat);
      }

      virtual void SetCameraToClipMatrix(const math::Matrix4& mat) override
      {
         ShaderProgram->SetFloats("ToClip", mat);
      }

      virtual void SetCameraPosition(const math::Vector3& position) override
      { 
         ShaderProgram->SetFloats("CameraPosition", position);
      }

      virtual void SetSpotlightsCount(const size_t count) override
      {
         ShaderProgram->SetInt("SpotlightsCount", count);
      }

      virtual void SetPointLightsCount(const size_t count) override
      {
         ShaderProgram->SetInt("PointLightsCount", count);
      }


      //This functions are used by the render to get infomation about shader
   
      virtual size_t GetPositionAttrib() const override
      {
         return 0;
      }

      virtual size_t GetNormalAttrib() const override
      {
         return 1;
      }

      virtual size_t GetUvAttrib() const override
      {
         return 2;
      }

      virtual size_t GetTangentAttrib() const override
      {
         return 3;
      }

      virtual std::string_view GetLightUBO() const override
      {
         return "LightBlock";
      }
   }; 
}
