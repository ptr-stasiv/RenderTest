#pragma once
#include "GL/glew.h"

#include "../math/matrices/matrix4x4.h"

char* ReadFile(const char* filepath);

GLuint CreateShader(const GLenum shaderType, const char* shaderSrc);

void SetShaderMatrix4(const GLuint program, const char* paramName, const Matrix4 matrix);
void SetShaderVector4(const GLuint program, const char* paramName, const Vector4 vector);
void SetShaderVector3(const GLuint program, const char* paramName, const Vector3 vector);
void SetShaderInt(const GLuint program, const char* paramName, const int scalar);