#version 460 core

out vec4 OutColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 CameraPosition;

vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

float ambientIntensity = 0.08f;

vec3 specularIntensity = vec3(1.0f, 1.0f, 1.0f); 
float specularExponent = 256.0f;

void main()
{
    vec3 ambientComponent = lightColor * ambientIntensity;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diffCof = max(dot(Normal, lightDir), 0.0f);
    vec3 diffuseComponent = lightColor * diffCof;

    vec3 viewDir = normalize(CameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = 0.5f * spec * lightColor;  

    vec3 resColor = vec3(1.0f, 0.2f, 0.2f) * (ambientComponent + diffuseComponent + specular);
    OutColor = vec4(resColor, 1.0f);
}