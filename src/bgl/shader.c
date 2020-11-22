#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

char* ReadFile(const char* filepath)
{
   FILE* file = fopen(filepath, "rb");
   if (!file) //Replace with assert
      return NULL;

   fseek(file, 0, SEEK_END);
   long count = ftell(file);
   rewind(file);

   char* buffer = malloc(count);
   if (!buffer) //Replace with assert
      return NULL;

   fread(buffer, 1, count, file);
   buffer[count] = '\0';

   fclose(file);

   return buffer;
}

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
