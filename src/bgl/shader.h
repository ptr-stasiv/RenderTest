#pragma once
#include "GL/glew.h"

char* ReadFile(const char* filepath);

GLuint CreateShader(const GLenum shaderType, const char* shaderSrc);
