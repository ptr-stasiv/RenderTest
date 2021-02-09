#pragma once
#include "GL/glew.h"

#include "../math/matrices/matrix4x4.h"

namespace bgl
{
   GLuint CreateShader(const GLenum shaderType, const char* shaderSrc);

   void SetShaderMatrix4(const GLuint program, const char* paramName, const math::Matrix4 matrix);
   void SetShaderVector4(const GLuint program, const char* paramName, const math::Vector4 vector);
   void SetShaderVector3(const GLuint program, const char* paramName, const math::Vector3 vector);
   void SetShaderInt(const GLuint program, const char* paramName, const int scalar);
   void SetShaderFloat(const GLuint program, const char* paramName, const float scalar);
}