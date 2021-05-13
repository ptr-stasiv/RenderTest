#version 460 core

layout(local_size_x = 1) in;

uniform mat4 ToCamera;
uniform mat4 ToClip;

uniform vec2 TileSize;

struct Tile
{
    vec2 Position;
    int LightsCount;
};

struct BoundingSphere
{
    vec3 Center;
    vec3 Point;
};

layout(std430) buffer tilesBuffer
{
    Tile Tiles [];
};

layout(std430) buffer lightBuffer
{
    BoundingSphere SphereList[];
};

void main()
{
    vec4 clipCenter = ToClip * ToCamera * vec4(SphereList[0].Center, 1.0f);
    vec4 clipPoint = ToClip * ToCamera * vec4(SphereList[0].Point, 1.0f);

    vec3 ndcCenter = clipCenter.xyz / clipCenter.w;
    vec3 ndcPoint = clipPoint.xyz / clipPoint.w;

    float r = length(ndcPoint.xy - ndcCenter.xy);

    Tile curTile = Tiles[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x];

    vec2 bottomLeft = curTile.Position;
    vec2 topLeft = vec2(bottomLeft.x, bottomLeft.y + TileSize.y);
    vec2 topRight = vec2(bottomLeft.x + TileSize.x, bottomLeft.y + TileSize.y);
    vec2 bottomRight = vec2(bottomLeft.x + TileSize.x, bottomLeft.y);

    float dist1 = length(bottomLeft - ndcCenter.xy);
    float dist2 = min(length(topLeft - ndcCenter.xy), dist1);
    float dist3 = min(length(topRight - ndcCenter.xy), dist2);
    float dist4 = min(length(bottomRight - ndcCenter.xy), dist3);

    if (dist4 <= r)
        Tiles[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x].LightsCount = 1;
    else
        Tiles[gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x].LightsCount = 0;
}