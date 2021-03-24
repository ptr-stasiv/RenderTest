#version 460 core

#extension GL_ARB_bindless_texture: enable

out vec4 Color;

in vec2 Uv;

layout(bindless_sampler) uniform sampler2D Texture;

void main()
{
   Color = texture(Texture, Uv);
}