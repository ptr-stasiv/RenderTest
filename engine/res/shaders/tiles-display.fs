#version 460 core

out vec4 OutColor;

in vec2 UV;

uniform ivec2 TilesCount;
uniform vec2 TileSize;
uniform float WindowAR;

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
    vec3 color;

    vec2 cUV = vec2(UV.x * WindowAR, UV.y);

    int idX = int(floor(cUV.x / TileSize.x));
    int idY = int(floor(cUV.y / TileSize.y));

    if(Tiles[idX + idY * TilesCount.x].LightsCount > 0)
        color.x = Tiles[idX + idY * TilesCount.x].LightsCount;
    else
        color.x = 0.0f;

    OutColor = vec4(color, 0.5f);
}