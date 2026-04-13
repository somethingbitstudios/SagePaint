#include "./ShaderProgram.h"


ShaderProgram::ShaderProgram(ShaderPtr vert,ShaderPtr frag)
{
	id = glCreateProgram();
	glAttachShader(id, vert->id);
	glAttachShader(id, frag->id);
	glLinkProgram(id);

}

void ShaderProgram::Dispose() {
	glDeleteProgram(id);
}