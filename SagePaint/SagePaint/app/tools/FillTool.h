#pragma once
#include <string>
#include <nlohmann/json.hpp>
class FillTool {
public:
	static void Fill();
	static void FillRender(unsigned char* texture, int w, int h, int x, int y, unsigned char color[4]);
	static void ShowUI();
	static std::string ConfigString();
	static void LoadConfig(const nlohmann::json& j);
	static float opacity;
};