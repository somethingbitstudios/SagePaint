#pragma once
#include <glm/ext/vector_int2.hpp>

#include <nlohmann/json.hpp>
#include <string>
enum ShapeMode {
	SHAPE_NORMAL,
	SHAPE_INRECT,
	SHAPE_FAST
};
class ShapeTool {
public:
	static void ShapeStart();
	static void ShapePreview();
	static void ShapeRender(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4],ShapeMode mode);
	static void ShapeRenderFilledRect(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4]);
	static void ShowUI();

	static std::string ConfigString();
	static void LoadConfig(const nlohmann::json& j);
	static void ShapeEnd();
	static int pointNumber;
	static float angleOffset;
private:
	static glm::ivec2 downPos;
	static float strokeSize;
	static ShapeMode mode;
};