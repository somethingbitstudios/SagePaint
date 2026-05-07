#include "./ShaderManager.h"

std::unordered_map<std::string, ShaderProgramPtr> ShaderManager::programs;

ShaderManager::ShaderManager()
{
	Init();
}

ShaderManager::~ShaderManager()
{
	Dispose();
}

ShaderProgramPtr ShaderManager::Get(std::string name)
{
	auto it = programs.find(name);
	if (it != programs.end())
	{
		return it->second;
	}
	return nullptr; 
}


void ShaderManager::Init()
{
	ShaderPtr basic_vert = std::make_shared<Shader>("./shaders/standard.vert");
	
	ShaderPtr basic_frag = std::make_shared<Shader>("./shaders/standard.frag");
	Create("STANDARD", basic_vert, basic_frag);
	basic_frag->Dispose();


	ShaderPtr canvas_compositing_normal = std::make_shared<Shader>("./shaders/canvas/compositing/normal.frag");
	Create("CANVAS_COMPOSITE_NORMAL", basic_vert, canvas_compositing_normal);
	canvas_compositing_normal->Dispose();

	ShaderPtr canvas_compositing_lighten = std::make_shared<Shader>("./shaders/canvas/compositing/lighten.frag");
	Create("CANVAS_COMPOSITE_LIGHTEN", basic_vert, canvas_compositing_lighten);
	canvas_compositing_lighten->Dispose();

	ShaderPtr canvas_compositing_darken = std::make_shared<Shader>("./shaders/canvas/compositing/darken.frag");
	Create("CANVAS_COMPOSITE_DARKEN", basic_vert, canvas_compositing_darken);
	canvas_compositing_darken->Dispose();

	//template if I want to add more complex shader
	// WOULD require pingpong framebuffer though, seems annoying
	//ShaderPtr canvas_compositing_lighten = std::make_shared<Shader>("./shaders/canvas/compositing/lighten.frag");

	//Create("CANVAS_COMPOSITE_LIGHTEN", basic_vert, canvas_compositing_lighten);
	//canvas_compositing_lighten->Dispose();


	ShaderPtr canvas_final = std::make_shared<Shader>("./shaders/canvas/final.frag");

	Create("CANVAS_FINAL", basic_vert, canvas_final);
	canvas_final->Dispose();


	basic_vert->Dispose();
}

void ShaderManager::Dispose()
{
	//...
}

void ShaderManager::Create(std::string name, ShaderPtr vert, ShaderPtr frag)
{
	ShaderProgramPtr p = std::make_shared<ShaderProgram>(vert, frag);
	programs.try_emplace(name, p);
}
