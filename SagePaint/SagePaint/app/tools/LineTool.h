#pragma once
#include <glm/ext/vector_int2.hpp>

class LineTool {
public:
	static void LineStart();
	static void LinePreview();
	static void LineRender(unsigned char* texture, int tex_w, int tex_h, int x0, int y0, int x1, int y1, unsigned char color[4]);
	static void LineEnd();
private:
	static glm::ivec2 downPos; 
}; 