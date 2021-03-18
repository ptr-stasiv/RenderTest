#version 460 core

#define MAX_POINT_LIGHTS 32

out vec4 FragColor;

in VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 UV;
} vs_in;

struct PointLight
{
    vec4 Position;
    vec4 Color;

    float Quadratic;
    float Linear;
    float Constant;
};

//layout(std140, binding = 0) uniform LightBlock
//{
//    PointLight PointLightArray[MAX_POINT_LIGHTS];
//} lightBlock;
//
uniform int PointLightsCount;

void main()
{
    FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);
}