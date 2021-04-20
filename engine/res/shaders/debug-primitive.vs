#version 460 core

layout(location = 0) in vec3 position;

uniform mat4 ToClip = mat4(1.0f);
uniform mat4 ToCamera = mat4(1.0f);
uniform mat4 ToWorld = mat4(1.0f);

void main()
{
	gl_Position = ToClip * ToCamera * vec4(position, 1.0f);
}