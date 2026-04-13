#pragma once

#include "debug.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <unordered_map>



class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();
	static ShaderProgramPtr Get(std::string name);
	static void Init();
	static void Dispose();
	static void Create(std::string name, ShaderPtr vert, ShaderPtr frag);
private:
	static std::unordered_map<std::string, ShaderProgramPtr> programs;
};
typedef std::shared_ptr<ShaderManager> ShaderManagerPtr;