#pragma once 
#include <vector>

#include "GL/glew.h"

#include "../math/matrices/matrix4x4.h"

namespace gl
{
   using Program = GLint;
   using Shader  = GLint;

   inline Shader CreateShader(const GLenum shaderType, const char* shaderSrc)
   {
      GLuint resShader = glCreateShader(shaderType);
      glShaderSource(resShader, 1, &shaderSrc, NULL);
      glCompileShader(resShader);

      GLint errCode;

      glGetShaderiv(resShader, GL_COMPILE_STATUS, &errCode);

      //This later will be replaced with better error handling system
      if (!errCode)
      {
         char log[512];
         glGetShaderInfoLog(resShader, 512, NULL, log);
         printf(log);

         return -1;
      }

      return resShader;
   }

   inline Program CreateProgram()
   {
      return glCreateProgram();
   }
   
   inline void UseProgram(const Program program)
   {
      return glUseProgram(program);
   }

   inline void DispatchCompute(const GLuint numGroupsX = 1, GLuint numGroupsY = 1, const GLuint numGroupsZ = 1)
   {
      glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
   }
 
   inline void AttachShader(const Program program, const Shader shader)
   {
      return glAttachShader(program, shader);
   }

   inline void LinkProgram(const Program program)
   {
      return glLinkProgram(program);
   }

   inline void DeleteProgram(const Program program)
   {
      return glDeleteProgram(program);
   }

   inline void DeleteShader(const Shader shader)
   {
      return glDeleteShader(shader);
   }

   inline void SetShaderMatrix4(const Program program, const char* paramName, const math::Matrix4 matrix)
   {
      glUniformMatrix4fv(glGetUniformLocation(program, paramName), 1, GL_FALSE, &matrix.Data[0]);
   }

   inline void SetShaderVector4(const Program program, const char* paramName, const math::Vector4 vector)
   {
      glUniform4f(glGetUniformLocation(program, paramName), vector.x, vector.y, vector.z, vector.w);
   }

   inline void SetShaderVector3(const Program program, const char* paramName, const math::Vector3 vector)
   {
      glUniform3f(glGetUniformLocation(program, paramName), vector.x, vector.y, vector.z);
   }

   inline void SetShaderInt(const Program program, const char* paramName, const int scalar)
   {
      glUniform1i(glGetUniformLocation(program, paramName), scalar);
   }

   inline void SetShaderFloat(const Program program, const char* paramName, const float scalar)
   {
      glUniform1f(glGetUniformLocation(program, paramName), scalar);
   }
}
