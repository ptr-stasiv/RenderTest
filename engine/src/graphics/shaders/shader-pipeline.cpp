#include "shader-pipeline.h"

namespace graphics
{
   void ShaderPipeline::Compile()
   {
      for (uint8_t i = 0; i < g_ShadersTypeCount; ++i)
      {
         if (ShaderArr[i].empty())
            continue;

         GLenum oglShaderType = 0;

         switch (static_cast<ShaderType>(i))
         {
      #define X(e, v) case ShaderType::e: oglShaderType = v; break;
               SHADER_TYPE_LIST
      #undef X
         }

         GLuint shader = bgl::CreateShader(oglShaderType, ShaderArr[i].data());

         glAttachShader(ProgramId, shader);
         glDeleteShader(shader);
      }

      glLinkProgram(ProgramId);
   }
}