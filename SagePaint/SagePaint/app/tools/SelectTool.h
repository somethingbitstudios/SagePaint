#pragma once
#include <glm/ext/vector_int2.hpp>

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
private:
	static glm::ivec2 downPos;
	
};