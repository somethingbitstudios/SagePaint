#version 430 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vTex;

layout(location = 0) uniform mat4 MVP;

out vec2 TexCoord;
void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    TexCoord=vTex;
}