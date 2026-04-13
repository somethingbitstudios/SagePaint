#include "Shader.h"
#include "../file/FileManager.h"

Shader::Shader(std::string path)
{

	std::string shader_text = FileManager::LoadTextFile(path);
	const char* src = shader_text.c_str();
	if (path.ends_with(".vert")) {
		id = glCreateShader(GL_VERTEX_SHADER);
	}
	else {
		id = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
}
void Shader::Dispose()
{
	glDeleteShader(id);
}