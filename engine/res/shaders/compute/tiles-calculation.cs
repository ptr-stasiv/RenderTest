#version 460 core

layout(local_size_x = 1) in;

layout(std430) buffer tilesBuffer
{
    float Values[16];
};

void main()
{
    Values[gl_GlobalInvocationID.x] = 2.0f;
}