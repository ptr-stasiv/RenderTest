#version 460 core

#define MAX_LIGHTS  32

out vec4 OutColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 CameraPosition;

struct Light
{
    vec3 Position;
    vec3 Color;
};

uniform Light LightsArray[MAX_LIGHTS];
uniform int LightsCount = 0;

float ambientIntensity = 0.03;

vec3 specularIntensity = vec3(1.0f, 1.0f, 1.0f);
float specularExponent = 16.0f;

void main()
{
    vec3 resColor = vec3(0.0f);

    for(int i = 0; i < min(LightsCount, MAX_LIGHTS); ++i)
    {
        vec3 lightPos = LightsArray[i].Position;
        vec3 ligthColor = LightsArray[i].Color;

        vec3 ambientComponent = ligthColor * ambientIntensity;
        
        vec3 lightDir = normalize((lightPos - FragPos));

        float diffCof = max(dot(Normal, lightDir), 0.0f);
        vec3 diffuseComponent = ligthColor * diffCof;

        vec3 viewDir = normalize(CameraPosition - FragPos);
        vec3 halfwayVec = normalize(viewDir + lightDir);
        float specCof = pow(max(dot(Normal, halfwayVec), 0.0f), specularExponent);
        vec3 specularComponent = ligthColor * specCof * specularIntensity;

        resColor += vec3(1.0f, 0.2f, 0.2f) * (ambientComponent + diffuseComponent + specularComponent);
    }

    OutColor = vec4(resColor, 1.0f);
}