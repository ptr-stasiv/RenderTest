#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 UV;
	vec3 Tangent;
	vec3 Bitangent;
} vs_out;

uniform mat4 ToProjection = mat4(1.0f);
uniform mat4 ToCamera = mat4(1.0f);
uniform mat4 ToWorld = mat4(1.0f);

void main()
{
	vs_out.FragPos = (ToWorld * vec4(position, 1.0f)).xyz;
	vs_out.Normal = normal;
	vs_out.UV = uv;
	vs_out.Tangent = (ToWorld * vec4(tangent, 1.0f)).xyz;
	vs_out.Bitangent = (ToWorld * vec4(bitangent, 1.0f)).xyz;

	gl_Position = ToProjection * ToCamera * ToWorld * vec4(position, 1.0f);
}