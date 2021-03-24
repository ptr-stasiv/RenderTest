#version 460 core
      
layout(location = 0) in vec2 pos;

out vec2 Uv;

void main()
{
   //Mirror coordinates by x axis
   Uv = vec2(pos.x, -pos.y) * 0.5f + 0.5f;

   gl_Position = vec4(pos, 0.0f, 1.0f);
}