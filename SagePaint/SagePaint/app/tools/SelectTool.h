#pragma once
#include <glm/ext/vector_int2.hpp>

class SelectTool {
public:
	static void SelectStart();
	static void SelectPreview();
	static void SelectEnd();
	static bool dragMode;
	static void SelectDragRender();
	static void SelectDragCommit();
private:
	static glm::ivec2 downPos;
	
};