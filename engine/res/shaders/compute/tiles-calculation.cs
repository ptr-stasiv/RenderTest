#version 460 core

layout(local_size_x = 1) in;

uniform vec2 TileSize;

struct Tile
{
    vec2 Position;
    int LightsCount;
};

layout(std430) buffer tilesBuffer
{
    Tile Tiles[];
};

void main()
{
    //vec2 pos;
    //pos.x = -1.0f + gl_GlobalInvocationID.x * TileSize.x;
    //pos.y = -1.0f + gl_GlobalInvocationID.y * TileSize.y;

    uint id = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x;
    Tiles[id].LightsCount = 0;
    Tiles[id].Position = vec2(2.0f);
}
