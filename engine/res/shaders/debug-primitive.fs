#version 460 core

out vec4 OutputColor;

uniform vec3 LinesColor;

void main()
{
	OutputColor = vec4(LinesColor, 1.0f);
}