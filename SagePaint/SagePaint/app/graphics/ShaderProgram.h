#pragma once
#include "debug.h"
#include <GL/glew.h>
#include "Shader.h"


class ShaderProgram {
public:
	ShaderProgram(ShaderPtr vert,ShaderPtr frag);
	void Dispose();
	GLuint id;
};
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;