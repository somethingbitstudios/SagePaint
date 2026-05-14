#version 430 core

out vec4 fragment;
in vec2 TexCoord;

uniform sampler2D texDst; // The accumulated background
uniform sampler2D texSrc; // The new layer
uniform float opacity;    // Opacity of the new layer
uniform vec2 texSize;

void main()
{
    vec4 dst = texture(texDst, TexCoord);
    vec4 src = texture(texSrc, TexCoord);

    fragment = vec4(src.r*dst.r,src.g*dst.g,src.b*dst.b,src.a*dst.a);
}