#include "shader.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>

namespace bgl
{
   GLuint CreateShader(const GLenum shaderType, const char* shaderSrc)
   {
      GLuint resShader = glCreateShader(shaderType);
      glShaderSource(resShader, 1, &shaderSrc, NULL);
      glCompileShader(resShader);

      GLint errCode;

      glGetShaderiv(resShader, GL_COMPILE_STATUS, &errCode);
      if (!errCode)
      {
         char log[512];
         glGetShaderInfoLog(resShader, 512, NULL, log);
         printf(log);
      }

      return resShader;
   }

   void SetShaderMatrix4(const GLuint program, const char* paramName, const math::Matrix4 matrix)
   {
      glUniformMatrix4fv(glGetUniformLocation(program, paramName), 1, GL_FALSE, &matrix.Data[0]);
   }

   void SetShaderVector4(const GLuint program, const char* paramName, const math::Vector4 vector)
   {
      glUniform4f(glGetUniformLocation(program, paramName), vector.x, vector.y, vector.z, vector.w);
   }

   void SetShaderVector3(const GLuint program, const char* paramName, const math::Vector3 vector)
   {
      glUniform3f(glGetUniformLocation(program, paramName), vector.x, vector.y, vector.z);
   }

   void SetShaderInt(const GLuint program, const char* paramName, const int scalar)
   {
      glUniform1i(glGetUniformLocation(program, paramName), scalar);
   }

   void SetShaderFloat(const GLuint program, const char* paramName, const float scalar)
   {
      glUniform1f(glGetUniformLocation(program, paramName), scalar);
   }
}