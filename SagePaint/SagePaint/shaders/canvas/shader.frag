#version 330
out vec4 fragment;
in vec2 TexCoord;
uniform sampler2D tex;
void main()
{
    fragment = texture(tex,TexCoord);
}