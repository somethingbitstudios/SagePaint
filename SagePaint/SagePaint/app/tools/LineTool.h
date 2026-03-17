#pragma once
#include <glm/ext/vector_int2.hpp>

class LineTool {
public:
	static void LineStart();
	static void LinePreview();
	static void LineEnd();
private:
	static void LineRender();
	static glm::ivec2 downPos;
};