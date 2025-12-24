#version 330

in vec2 TexCoord;
out vec4 fragment;

uniform sampler2D tex;
uniform vec2 texSize;   // texture resolution in texels

// Catmull-Rom spline
float cubic(float v)
{
    v = abs(v);
    if (v <= 1.0)
        return 1.0 - 2.0 * v * v + v * v * v;
    else if (v < 2.0)
        return 4.0 - 8.0 * v + 5.0 * v * v - v * v * v;
    return 0.0;
}

vec4 textureBicubic(sampler2D s, vec2 uv)
{
    vec2 texel = 1.0 / texSize;
    vec2 coord = uv * texSize - 0.5;

    vec2 f = fract(coord);
    coord -= f;

    vec4 sum = vec4(0.0);

    for (int j = -1; j <= 2; ++j)
    {
        for (int i = -1; i <= 2; ++i)
        {
            vec2 offset = vec2(i, j);
            vec2 sampleUV = (coord + offset + 0.5) * texel;

            float w = cubic(offset.x - f.x) * cubic(f.y - offset.y);
            sum += texture(s, sampleUV) * w;
        }
    }

    return sum;
}

void main()
{
    fragment = textureBicubic(tex, TexCoord);
}
