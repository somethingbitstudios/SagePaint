#pragma once
#include "debug.h"
#include <string>
#include <GL/glew.h>



class Shader {
public:
	Shader( std::string path);
	void Dispose();
	GLuint id;
};
typedef std::shared_ptr<Shader> ShaderPtr;