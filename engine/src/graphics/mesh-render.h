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
      math::Vector4 Rotation; //This later will be replaced
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
      size_t InstanceId = -1;
   public:
      std::shared_ptr<ShaderProgram> ShaderProgram;

      BaseMaterial(const std::string_view& vertexShaderPath, const std::string_view& fragmentShaderPath);

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
   };

   class PhongMaterial : public BaseMaterial
   {
   public:
      PhongMaterial();

      math::Vector4 Diffuse;

      math::Vector3 Specular;

      math::Vector3 Emissive;

      float Glossiness;

      std::shared_ptr<Texture2D>  DiffuseTexture;
      std::shared_ptr<Texture2D>  SpecularTexture;
      std::shared_ptr<Texture2D>  NormalTexture;
      std::shared_ptr<Texture2D>  EmissiveTexture;
      std::shared_ptr<Texture2D>  GlossinessTexture;


      virtual void ResolveUniforms() override;

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
   }; 
}
