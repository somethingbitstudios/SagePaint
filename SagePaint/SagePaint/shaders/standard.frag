#version 430 core

out vec4 fragment;
in vec2 TexCoord;

uniform sampler2D tex;


void main()
{
/*
    vec4 texel = texture(tex, TexCoord);
    fragment = vec4(texel.rgb,texel.a*opacity);
    */
    
    fragment = texture(tex, TexCoord);
}