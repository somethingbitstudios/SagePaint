#version 430 core
out vec4 fragment;
in vec2 TexCoord;
uniform sampler2D tex;
uniform float opacity;
uniform vec2 texSize;
void main()
{
    vec4 texel = texture(tex, TexCoord);
    texel.a *=opacity;
    fragment = texel;
   
}