#include "graphics/shaders/shader-program.h"

#include "platforms/opengl/primitives/shaders/shader.h"
#include "debug/log/log.h"

namespace graphics
{
   static constexpr const uint8_t ShadersTypeCount = static_cast<uint8_t>(ShaderType::LAST_ENUM_ELEMENT);

#define SHADER_TYPE_LIST \
   X(Vertex, GL_VERTEX_SHADER) \
   X(Fragment, GL_FRAGMENT_SHADER) 

   struct ShaderProgram::NativeInfo
   {
      gl::Program ProgramId;
      std::string_view ShaderArr[ShadersTypeCount];
   };

   ShaderProgram::ShaderProgram()
   {
      Native = std::make_unique<NativeInfo>();
      Native->ProgramId = gl::CreateProgram();
   }

   ShaderProgram::~ShaderProgram()
   {
      gl::DeleteProgram(Native->ProgramId);
   }

   void  ShaderProgram::Add(const ShaderType shaderType, const std::string_view& shaderSrc)
   {
      if (static_cast<uint8_t>(shaderType) >= ShadersTypeCount)
      {
         LOG_ERROR("Shader pipeline error!");
         return;
      }

      Native->ShaderArr[static_cast<uint8_t>(shaderType)] = shaderSrc;
   }

   void ShaderProgram::Compile()
   {
      for (uint8_t i = 0; i < ShadersTypeCount; ++i)
      {
         if (Native->ShaderArr[i].empty())
            continue;

         GLenum oglShaderType = 0;

         switch (static_cast<ShaderType>(i))
         {
      #define X(e, v) case ShaderType::e: oglShaderType = v; break;
               SHADER_TYPE_LIST
      #undef X
             
         default:
            continue;
         }

         GLuint shader = gl::CreateShader(oglShaderType, Native->ShaderArr[i].data());

         gl::AttachShader(Native->ProgramId, shader);
         gl::DeleteShader(shader);
      }

      gl::LinkProgram(Native->ProgramId);
   }

   void ShaderProgram::Use() const
   {
      gl::UseProgram(Native->ProgramId);
   }

   void ShaderProgram::SetFloats(const std::string_view& name, const math::Matrix4& m) const
   {
      gl::SetShaderMatrix4(Native->ProgramId, &name[0], m);
   }

   void ShaderProgram::SetFloats(const std::string_view& name, const math::Vector4& v) const
   {
      gl::SetShaderVector4(Native->ProgramId, &name[0], v);
   }

   void ShaderProgram::SetFloats(const std::string_view& name, const math::Vector3& v) const
   {
      gl::SetShaderVector3(Native->ProgramId, &name[0], v);
   }

   void ShaderProgram::SetFloat(const std::string_view& name, const float s) const
   {
      gl::SetShaderFloat(Native->ProgramId, &name[0], s);
   }

   void ShaderProgram::SetInt(const std::string_view& name, const int32_t s) const
   {
      gl::SetShaderInt(Native->ProgramId, &name[0], s);
   }
}
