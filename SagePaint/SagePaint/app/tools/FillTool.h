#pragma once
#include <string>
#include <nlohmann/json.hpp>
enum FillMode {
	FILL_SCANLINE,
	FILL_4WAY
};
class FillTool {
	
public:
	static void Fill();
	static void FillRender(unsigned char* texture, int w, int h, int x, int y, unsigned char color[4],FillMode fm);
	static void ShowUI();
	static std::string ConfigString();
	static void LoadConfig(const nlohmann::json& j);
	static float opacity;
	static FillMode mode;
};