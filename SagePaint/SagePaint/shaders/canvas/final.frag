#version 430 core
out vec4 fragment;
in vec2 TexCoord;
in vec2 CanvasXAndInverseAspectRatio;
uniform sampler2D tex;
//uniform float opacity;
uniform vec2 texSize;
void main()
{
    vec4 texel = texture(tex, TexCoord);
   
    // checkerboard scale
    float scaleX = texSize.x;
    float aspectRatio =  texSize.y;

    vec2 initial = TexCoord;
     initial.y *=aspectRatio;

    vec2 grid = floor(initial * scaleX);
    float checker = mod(grid.x + grid.y, 2.0);

    vec3 background = mix(vec3(0.975), vec3(0.8), checker);
  
  fragment = vec4(texel.rgb + background * (1.0 - texel.a), 1.0); 

}