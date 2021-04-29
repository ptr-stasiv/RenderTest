#version 460 core

layout(local_size_x = 1) in;

struct Tile
{
    vec2 Position;
    vec2 Size;
};

layout(std430) buffer tilesBuffer
{
    Tile Tiles[];
};

void main()
{
    Tiles[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x].Position = vec2(1.0f);
}