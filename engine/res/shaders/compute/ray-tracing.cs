#version 460 core

layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img;

struct Sphere
{
    vec3 Position;
    float Radius;
};

void main()
{
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec3 resColor;

    vec3 center = vec3(gl_NumWorkGroups.x / 2, gl_NumWorkGroups.y / 2, 100.0f);
    float radius = 250.0f;

    vec3 dir = vec3(pixelCoord.x, pixelCoord.y, center.z);

    //Light
    vec3 lightPos = vec3(gl_NumWorkGroups.x / 2 + 200.0f, gl_NumWorkGroups.y / 2 - 700.0f, -200.0f);
    vec3 lightDir = normalize(lightPos - dir);

    vec3 normal = normalize(dir - center);

    vec3 diff = vec3(1.0f) * clamp(dot(lightDir, normal), 0.0f, 1.0f);

    if (length(dir - center) <= radius)
        resColor = vec3(1.0f, 0.2f, 0.2f) * diff;
    else
        resColor = vec3(0.5f, 0.5f, 0.5f);

    imageStore(img, pixelCoord, vec4(resColor, 1.0f));
}