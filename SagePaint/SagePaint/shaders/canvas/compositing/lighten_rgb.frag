#version 430 core

out vec4 fragment;
in vec2 TexCoord;

uniform sampler2D texDst; 
uniform sampler2D texSrc; 
uniform float opacity;   
uniform vec2 texSize;

void main()
{
    vec4 dst = texture(texDst, TexCoord);
    vec4 src = texture(texSrc, TexCoord);


    src.a *= opacity;

    float outAlpha = src.a + dst.a * (1.0 - src.a);

    vec3 outRGB;
    if (outAlpha > 0.0) {
        vec3 blendedRGB = max(src.rgb, dst.rgb);
        
        outRGB = (src.rgb * src.a * (1.0 - dst.a) + 
                  blendedRGB * src.a * dst.a + 
                  dst.rgb * dst.a * (1.0 - src.a)) / outAlpha;
    } else {
        outRGB = vec3(0.0);
    }

    fragment = vec4(outRGB, outAlpha);
}