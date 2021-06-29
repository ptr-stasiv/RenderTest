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
    vec3 Tangent;
} vs_in;

uniform vec3 CameraPosition;

uniform int PointLightsCount;
uniform int SpotlightsCount;

uniform vec4 Diffuse;
        
uniform vec3 Specular;
        
uniform vec3 Emissive;
        
uniform float Glossiness;

layout(bindless_sampler) uniform sampler2D DiffuseTexture;
layout(bindless_sampler) uniform sampler2D NormalTexture;

layout(bindless_sampler) uniform sampler2D ShadowMaps[MAX_SPOTLIGHTS];

struct PointLight
{
    vec4 Position;
    vec4 Color;

    float Stretch;
    float Offset;
};

struct Spotlight
{
    mat4 Camera;

    vec4 Position;
    vec4 Direction;
    vec4 Color;

    float InnnerAngle;
    float OuterAngle;

    int ShadowMapId;
};

layout(std140) uniform LightBlock
{
    PointLight PointLightArray[MAX_POINT_LIGHTS];
    Spotlight SpotlightArray[MAX_SPOTLIGHTS];
} lightBlock;

float CalculateSpotlightShadow(in int id)
{
    vec4 lightSpaceFrag = lightBlock.SpotlightArray[id].Camera * vec4(vs_in.FragPos, 1.0f);

    vec3 proj = lightSpaceFrag.xyz / lightSpaceFrag.w;

    proj = proj * 0.5f + 0.5f;

    float fragDepth = proj.z;
    float shadowDepth = texture(ShadowMaps[id], proj.xy).r;

    float bias = 0.005f;

    return (fragDepth - bias > shadowDepth ? 1.0f : 0.0f);
}

vec3 CalculatePhong(in vec3 lightColor, in vec3 specular, in float gloss, in vec3 emissive, 
                    in vec3 lightDir, in vec3 normal, in vec3 viewDir, in float shadow)
{
    vec3 ambientComponent = lightColor * 0.1f;

    float diffC = max(dot(lightDir, normal), 0.0f);
    vec3 diffuseComponent  = lightColor * diffC * (1.0f - shadow);

    vec3 halfVector = normalize(viewDir + lightDir);
    float specC = pow(max(dot(halfVector, normal), 0.0f), gloss);
    vec3 specularComponent = lightColor * specC * specular;

    return ambientComponent + diffuseComponent + specularComponent + emissive;
}

void main()
{
    vec3 lightSum;

    vec3 normalTexture = 2.0f * (texture(NormalTexture, vs_in.UV).xyz - 0.5f);

    mat3 tbn = mat3(normalize(vs_in.Tangent),
                    normalize(cross(vs_in.Tangent, vs_in.Normal)),
                    normalize(vs_in.Normal));

    vec3 normal = vs_in.Normal;

    vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);


    for(int i = 0; i < min(PointLightsCount, MAX_POINT_LIGHTS); ++i)
    {
        vec3 lightPos = lightBlock.PointLightArray[i].Position.xyz;
        vec3 lightDir = normalize(lightPos - vs_in.FragPos);
        vec3 lightColor = lightBlock.PointLightArray[i].Color.xyz;

        float stretch = lightBlock.PointLightArray[i].Stretch;
        float offset = lightBlock.PointLightArray[i].Offset;
        
        vec3 phong = CalculatePhong(lightColor, Specular, Glossiness, Emissive, 
                                    lightDir, normal, viewDir, 0.0f);
        float lightDist = abs(length(lightPos - vs_in.FragPos));
        float attentuation = (1 / pow(offset + lightDist, 2)) * stretch;

        lightSum += phong * attentuation;
    }

    for(int i = 0; i < min(SpotlightsCount, MAX_SPOTLIGHTS); ++i)
    {
        vec3 lightPos = lightBlock.SpotlightArray[i].Position.xyz;
        vec3 spotDir = lightBlock.SpotlightArray[i].Direction.xyz;
        vec3 lightDir = normalize(lightPos - vs_in.FragPos);
        vec3 lightColor = lightBlock.SpotlightArray[i].Color.xyz;

        float innerAngle = lightBlock.SpotlightArray[i].InnnerAngle;
        float outerAngle = lightBlock.SpotlightArray[i].OuterAngle;

        float shadow = CalculateSpotlightShadow(i);

        vec3 phong = CalculatePhong(lightColor, Specular, Glossiness, Emissive, 
                                    lightDir, normal, viewDir, shadow);

        
        float currentA = outerAngle - acos(dot(spotDir, -lightDir));
        float diff = outerAngle - innerAngle;

        float attentuation = clamp(currentA / diff, 0.0f, 1.0f);
        lightSum += phong * attentuation;
    }

    float shadow = CalculateSpotlightShadow(0);

    FragColor = texture(DiffuseTexture, vs_in.UV) * vec4(lightSum, 1.0f);
}