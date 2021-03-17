#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

out VS_OUT
{
	vec3 Position;
	vec3 Normal;
	vec2 UV;
} vs_out;

uniform mat4 projection = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 model = mat4(1.0f);

void main()
{
	vs_out.Position = position;
	vs_out.Normal = normal;
	vs_out.UV = uv;

	gl_Position = projection * view * model * vec4(position, 1.0f);
}