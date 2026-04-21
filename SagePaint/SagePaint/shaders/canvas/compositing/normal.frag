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

    // Apply the layer opacity to the source alpha
    src.a *= opacity;

    // Calculate the final alpha
    float outAlpha = src.a + dst.a * (1.0 - src.a);

    vec3 outRGB;
    if (outAlpha > 0.0) {
        // Standard straight-alpha "Normal" blend math
        outRGB = (src.rgb * src.a + dst.rgb * dst.a * (1.0 - src.a)) / outAlpha;
    } else {
        outRGB = vec3(0.0);
    }

    fragment = vec4(outRGB, outAlpha);
}