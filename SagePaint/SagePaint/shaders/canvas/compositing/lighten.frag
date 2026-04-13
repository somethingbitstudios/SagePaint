#version 430 core
out vec4 fragment;

in vec2 TexCoord;

uniform sampler2D tex;      
uniform sampler2D bgTex;    
uniform float opacity;
uniform vec2 texSize;

void main()
{
    vec4 src = texture(tex, TexCoord);
    src.a *= opacity;
    vec4 dst = texture(bgTex, TexCoord);
    
    vec3 lightenColor = max(src.rgb, dst.rgb);
    
    vec3 finalColor = mix(dst.rgb, lightenColor, src.a);
    float finalAlpha = src.a + dst.a * (1.0 - src.a);

    fragment = vec4(finalColor, finalAlpha);
}