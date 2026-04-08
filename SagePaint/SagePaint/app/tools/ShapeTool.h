#pragma once
#include <glm/ext/vector_int2.hpp>

class ShapeTool {
public:
	static void ShapeStart();
	static void ShapePreview();
	static void ShapeRender(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4],bool rectConstrained,bool fastRect);
	static void ShapeRenderFilledRect(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4]);
	
	static void ShapeEnd();
	static bool rectConstrained;
	static unsigned int pointNumber;
	static float angleOffset;
	static bool fastRect;
private:
	static glm::ivec2 downPos;
};