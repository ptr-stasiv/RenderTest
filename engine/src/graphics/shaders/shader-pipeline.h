#pragma once
#include <string_view>

#include "bgl/shader.h"
#include "math/matrices/matrix4x4.h"
#include "math/vectors/vector4.h"
#include "math/vectors/vector3.h"

#include "debug/log/log.h"

namespace graphics
{
#define SHADER_TYPE_LIST \
   X(Vertex, GL_VERTEX_SHADER) \
   X(Fragment, GL_FRAGMENT_SHADER)  
   

   enum class ShaderType : uint8_t
   {
#define X(e, v) e,
      SHADER_TYPE_LIST
#undef X

      LAST_ENUM_ELEMENT
   };

   static constexpr const uint8_t g_ShadersTypeCount = static_cast<uint8_t>(ShaderType::LAST_ENUM_ELEMENT);


   struct ShaderPipeline
   {
   private:
      GLuint ProgramId;
      std::string_view ShaderArr[g_ShadersTypeCount];

      bool Created = false; //Temporal solution, this will be changed with more complex logging system
   public:
      inline void Create()
      {
         ProgramId = glCreateProgram();

         Created = true;

         //TODO error handling
      }

      inline void Destroy()
      {
         if (Created)
            glDeleteProgram(ProgramId);
      }

      inline void Add(const ShaderType shaderType, const std::string_view& shaderSrc)
      {
         if (static_cast<uint8_t>(shaderType) >= g_ShadersTypeCount)
         {
            LOG_ERROR("Shader pipeline error!");
            return;
         }

         ShaderArr[static_cast<uint8_t>(shaderType)] = shaderSrc;
      }

      void Compile();

      inline void Use() const
      {
         glUseProgram(ProgramId);
      }

      inline void SetFloats(const std::string_view& name, const math::Matrix4& m) const
      {
         bgl::SetShaderMatrix4(ProgramId, &name[0], m);
      }

      inline void SetFloats(const std::string_view& name, const math::Vector4& v) const
      {
         bgl::SetShaderVector4(ProgramId, &name[0], v);
      }

      inline void SetFloats(const std::string_view& name, const math::Vector3& v) const
      {
         bgl::SetShaderVector3(ProgramId, &name[0], v);
      }

      inline void SetFloat(const std::string_view& name, const float s) const
      {
         bgl::SetShaderFloat(ProgramId, &name[0], s);
      }

      inline void SetInt(const std::string_view& name, const int32_t s) const
      {
         bgl::SetShaderInt(ProgramId, &name[0], s);
      }
   };
}