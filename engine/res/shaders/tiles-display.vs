#version 460 core

layout(location = 0) in vec3 pos;

out vec2 UV;

void main()
{
	UV = pos.xy * 0.5f + 0.5f;
	gl_Position = vec4(pos, 1.0f);
}