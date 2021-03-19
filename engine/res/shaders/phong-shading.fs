#version 460 core

#define MAX_POINT_LIGHTS 32

out vec4 FragColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 UV;
} vs_in;

uniform vec3 CameraPosition;

struct PointLight
{
    vec4 Position;
    vec4 Color;

    float Quadratic;
    float Linear;
    float Constant;
};

layout(std140) uniform LightBlock
{
    PointLight PointLightArray[MAX_POINT_LIGHTS];
} lightBlock;

uniform int PointLightsCount;


vec3 CalculatePhong(in vec3 lightColor, in vec3 specular, in vec3 lightDir, in vec3 normal, in vec3 viewDir)
{
    vec3 ambientComponent = lightColor * 0.03f;

    float diffC = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseComponent  = lightColor * diffC;

    vec3 halfVector = normalize(viewDir + lightDir);
    float specC = max(pow(dot(halfVector, normal), 8.0f), 0.0f);
    vec3 specularComponent = lightColor * specC * specular;

    return ambientComponent + diffuseComponent + specularComponent;
}

void main()
{
    vec3 fragSum;

    vec3 normal = normalize(vs_in.Normal);

    vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);


    for(int i = 0; i < PointLightsCount; ++i)
    {
        vec3 lightDir = normalize(lightBlock.PointLightArray[i].Position.xyz - vs_in.FragPos);
        vec3 lightColor = lightBlock.PointLightArray[i].Color.xyz;

        fragSum += vec3(0.5f, 0.5f, 0.5f) * CalculatePhong(lightColor, vec3(1.0f), lightDir, normal, viewDir);
    }

    FragColor = vec4(fragSum, 1.0f);
}