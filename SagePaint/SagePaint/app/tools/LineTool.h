#pragma once
#include <glm/ext/vector_int2.hpp>

#include <nlohmann/json.hpp>
#include <string>
enum LineMode {
	LINE_NORMAL,
	LINE_RECT
};
class LineTool {
public:
	static void LineStart();
	static void LinePreview();
	static void LineRender(unsigned char* texture, int tex_w, int tex_h, int x0, int y0, int x1, int y1, float size, unsigned char color[4]);
	static void LineEnd();
	static void LoadConfig(const nlohmann::json& j);
	static std::string ConfigString();
	static void ShowUI();
private:
	static glm::ivec2 downPos; 
	static float strokeSize;
	static LineMode mode;
}; 