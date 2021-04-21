#version 460 core

out vec4 OutputColor;

uniform vec4 LinesColor;

void main()
{
	OutputColor = LinesColor;
}