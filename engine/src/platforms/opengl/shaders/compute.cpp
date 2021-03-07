#include "graphics/shaders/compute.h"

#include "platforms/opengl/primitives/shaders/shader.h"

namespace graphics
{
   struct ComputeShader::NativeInfo
   {
      gl::Program ProgramId;
   };

   ComputeShader::ComputeShader(const std::string_view& shaderSource)
   {
      gl::Shader shader = gl::CreateShader(GL_COMPUTE_SHADER, shaderSource.data());
    
      Native = std::make_unique<NativeInfo>();
      Native->ProgramId = gl::CreateProgram();

      gl::AttachShader(Native->ProgramId, shader);

      gl::LinkProgram(Native->ProgramId);

      gl::DeleteShader(shader);
   }

   ComputeShader::~ComputeShader()
   {
      gl::DeleteProgram(Native->ProgramId);
   }

   void ComputeShader::Use() const
   {
      gl::UseProgram(Native->ProgramId);
   }

   void ComputeShader::Dispatch(const uint32_t numGroupsX, uint32_t numGroupsY, const uint32_t numGroupsZ) const
   {
      Use();
      gl::DispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
   }

   void ComputeShader::SetFloats(const std::string_view& name, const math::Matrix4& m) const
   {
      gl::SetShaderMatrix4(Native->ProgramId, &name[0], m);
   }

   void ComputeShader::SetFloats(const std::string_view& name, const math::Vector4& v) const
   {
      gl::SetShaderVector4(Native->ProgramId, &name[0], v);
   }

   void ComputeShader::SetFloats(const std::string_view& name, const math::Vector3& v) const
   {
      gl::SetShaderVector3(Native->ProgramId, &name[0], v);
   }

   void ComputeShader::SetFloat(const std::string_view& name, const float s) const
   {
      gl::SetShaderFloat(Native->ProgramId, &name[0], s);
   }

   void ComputeShader::SetInt(const std::string_view& name, const int32_t s) const
   {
      gl::SetShaderInt(Native->ProgramId, &name[0], s);
   }
}
