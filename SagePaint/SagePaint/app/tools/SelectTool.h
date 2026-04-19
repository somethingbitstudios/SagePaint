#pragma once
#include <glm/ext/vector_int2.hpp>

#include <nlohmann/json.hpp>
#include <string>
class SelectTool {
public:
	static void SelectStart();
	static void SelectPreview();
	static void SelectEnd();
	static bool dragMode;
	static void SelectDragRender();
	static void SelectDragCommit();
	static void ShowUI();
	static std::string ConfigString();
	static void LoadConfig(const nlohmann::json& j);
private:
	static glm::ivec2 downPos;
	
};