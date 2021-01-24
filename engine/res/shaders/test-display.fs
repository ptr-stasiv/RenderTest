#version 460 core

#define MAX_LIGHTS    32
#define MAX_MATERIALS 32

out vec4 OutColor;

in VS_OUT
{
    vec3 Normal;
    vec3 FragPos;
    vec2 UV;
} vs_in;

uniform vec3 CameraPosition;

struct Light
{
    vec3 Position;
    vec3 Color;
};

struct PointLight
{
    vec3 Position;
    vec3 Color;

    float Linear;
    float Quadratic;
    float Constant;
};

struct Spotlight
{
    vec3 Position;
    vec3 Direction;
    vec3 Color;

    float OuterAngle;
    float InnerAngle;
};

struct Material
{
    vec3  Color;
    vec3  Specular;
    vec3  Emissive;
    float ShineExponent;
};

uniform bool UseDiffuseTexture = false;
uniform sampler2D DiffuseTexture;

uniform PointLight PointLightsArray[MAX_LIGHTS];
uniform int PointLightsCount = 0;

uniform Spotlight SpotlightsArray[MAX_LIGHTS];
uniform int SpotlightsCount = 0;

uniform Light LightsArray[MAX_LIGHTS];
uniform int LightsCount = 0;

uniform Material MaterialsArray[MAX_MATERIALS];
uniform int MaterialId;

float ambientIntensity = 0.03f;

void main()
{
    vec3 resColor = vec3(0.0f);

    vec3 objectColor;
    if(UseDiffuseTexture) 
        objectColor = texture(DiffuseTexture, vs_in.UV).xyz;
    else
        objectColor = MaterialsArray[MaterialId].Color;

    vec3 specularIntensity = MaterialsArray[MaterialId].Specular;
    vec3 emissive = MaterialsArray[MaterialId].Emissive;
    float specularExponent = MaterialsArray[MaterialId].ShineExponent;

    for(int i = 0; i <= min(LightsCount, MAX_LIGHTS); ++i)
    {
        vec3 lightPos   = LightsArray[i].Position;
        vec3 lightColor = LightsArray[i].Color;

        vec3 lightDir = normalize(lightPos - vs_in.FragPos);

        vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);

        vec3 ambientComponent = lightColor * ambientIntensity;

        float diffC = max(dot(vs_in.Normal, lightDir), 0);
        vec3 diffuseComponent = lightColor * diffC;

        vec3 halfwayVector = normalize(viewDir + lightDir);
        float specC = pow(max(dot(halfwayVector, vs_in.Normal), 0), specularExponent);
        vec3 specularComponent = lightColor * specC * specularIntensity;

        resColor += objectColor * (ambientComponent + diffuseComponent + specularComponent + emissive); 
    }

    for(int i = 0; i < min(PointLightsCount, MAX_LIGHTS); ++i)
    {
        vec3 lightPos   = PointLightsArray[i].Position;
        vec3 lightColor = PointLightsArray[i].Color;

        vec3 lightDir = normalize(lightPos - vs_in.FragPos);

        vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);

        vec3 ambientComponent = lightColor * ambientIntensity;

        float diffC = max(dot(vs_in.Normal, lightDir), 0);
        vec3 diffuseComponent = lightColor * diffC;

        vec3 halfwayVector = normalize(viewDir + lightDir);
        float specC = pow(max(dot(halfwayVector, vs_in.Normal), 0), specularExponent);
        vec3 specularComponent = lightColor * specC * specularIntensity;

        float linear = PointLightsArray[i].Linear;
        float quadratic = PointLightsArray[i].Quadratic;
        float constant = PointLightsArray[i].Constant;

        float lightDist = abs(length(lightPos - vs_in.FragPos));
        float attenuation = 1 / (constant + linear * lightDist + quadratic * lightDist * lightDist);

        resColor += objectColor * (ambientComponent + diffuseComponent + specularComponent + emissive) * attenuation; 
    }

    for(int i = 0; i < min(SpotlightsCount, MAX_LIGHTS); ++i)
    {   
        vec3 lightPos   = SpotlightsArray[i].Position;
        vec3 lightColor = SpotlightsArray[i].Color;

        vec3 lightDir = normalize(lightPos - vs_in.FragPos);

        vec3 viewDir = normalize(CameraPosition - vs_in.FragPos);

        vec3 ambientComponent = lightColor * ambientIntensity;

        float diffC = max(dot(vs_in.Normal, lightDir), 0);
        vec3 diffuseComponent = lightColor * diffC;

        vec3 halfwayVector = normalize(viewDir + lightDir);
        float specC = pow(max(dot(halfwayVector, vs_in.Normal), 0), specularExponent);
        vec3 specularComponent = lightColor * specC * specularIntensity;

        vec3 spotLightDir = SpotlightsArray[i].Direction;
        float cutOffAngle = SpotlightsArray[i].OuterAngle;
        float innOffAngle = SpotlightsArray[i].InnerAngle;

        float theta = acos(dot(spotLightDir, -lightDir));
        float diff = cutOffAngle - innOffAngle;
        float intensity = clamp((cutOffAngle - theta) / diff, 0.0f, 1.0f);

        resColor += objectColor * (ambientComponent + diffuseComponent + specularComponent + emissive) * intensity;
    }

    OutColor = vec4(resColor, 1.0f);
}