#version 460 core

#extension GL_ARB_bindless_texture: enable

#define MAX_POINT_LIGHTS 32
#define MAX_SPOTLIGHTS 32

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

    float Stretch;
    float Offset;
};

struct Spotlight
{
    vec4 Position;
    vec4 Direction;
    vec4 Color;

    float InnnerAngle;
    float OuterAngle;
};

layout(std140) uniform MaterialBlock
{
    vec4 DiffuseColor;
    
    vec4 SpecularColor;
    
    vec4 Emissive;

    float Glossiness;
} materialBlock;

layout(std140) uniform LightBlock
{
    PointLight PointLightArray[MAX_POINT_LIGHTS];
    Spotlight SpotlightArray[MAX_SPOTLIGHTS];
} lightBlock;

uniform int PointLightsCount;
uniform int SpotlightsCount;

layout(bindless_sampler) uniform sampler2D DiffuseTexture;
layout(bindless_sampler) uniform sampler2D SpecularTexture;
layout(bindless_sampler) uniform sampler2D NormalTexture;
layout(bindless_sampler) uniform sampler2D EmissiveTexture;
layout(bindless_sampler) uniform sampler2D GlossinessTexture;

vec3 CalculatePhong(in vec3 lightColor, in vec3 specular, in float gloss, in vec3 emissive, 
                    in vec3 lightDir, in vec3 normal, in vec3 viewDir)
{
    vec3 ambientComponent = lightColor * 0.1f;

    float diffC = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseComponent  = lightColor * diffC;

    vec3 halfVector = normalize(viewDir + lightDir);
    float specC = pow(max(dot(halfVector, normal), 0.0f), gloss);
    vec3 specularComponent = lightColor * specC * specular;

    return ambientComponent + diffuseComponent + specularComponent + emissive;
}

void main()
{
    vec3 lightSum;

    vec3 normal = normalize(vs_in.Normal);

    vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);

    for(int i = 0; i < min(PointLightsCount, MAX_POINT_LIGHTS); ++i)
    {
        vec3 lightPos = lightBlock.PointLightArray[i].Position.xyz;
        vec3 lightDir = normalize(lightPos - vs_in.FragPos);
        vec3 lightColor = lightBlock.PointLightArray[i].Color.xyz;

        float stretch = lightBlock.PointLightArray[i].Stretch;
        float offset = lightBlock.PointLightArray[i].Offset;
        
        vec3 phong = CalculatePhong(lightColor, materialBlock.SpecularColor.xyz, materialBlock.Glossiness, materialBlock.Emissive.xyz, 
                                    lightDir, normal, viewDir);
        float lightDist = abs(length(lightPos - vs_in.FragPos));
        float attentuation = (1 / pow(offset + lightDist, 2) * lightDist) * stretch;

        lightSum += phong * attentuation;
    }

    for(int i = 0; i < min(SpotlightsCount, MAX_POINT_LIGHTS); ++i)
    {
        vec3 lightPos = lightBlock.SpotlightArray[i].Position.xyz;
        vec3 spotDir = lightBlock.SpotlightArray[i].Direction.xyz;
        vec3 lightDir = normalize(lightPos - vs_in.FragPos);
        vec3 lightColor = lightBlock.SpotlightArray[i].Color.xyz;

        float innerAngle = lightBlock.SpotlightArray[i].InnnerAngle;
        float outerAngle = lightBlock.SpotlightArray[i].OuterAngle;

        vec3 phong = CalculatePhong(lightColor, materialBlock.SpecularColor.xyz, materialBlock.Glossiness, materialBlock.Emissive.xyz, 
                                    lightDir, normal, viewDir);

        vec3 halfwayVector = normalize(viewDir + lightDir);
        float specC = pow(max(dot(halfwayVector, normal), 0), 32.0f);
        vec3 specularComponent = lightColor * specC;

        
        float currentA = outerAngle - acos(dot(spotDir, -lightDir));
        float diff = outerAngle - innerAngle;

        float attentuation = clamp(currentA / diff, 0.0f, 1.0f);
        lightSum += lightDir * attentuation;
    }

    FragColor = texture(DiffuseTexture, vs_in.UV) * vec4(lightSum, 1.0f);
}