#pragma once
#include <string>
class FillTool {
public:
	static void Fill();
	static void FillRender(unsigned char* texture, int w, int h, int x, int y, unsigned char color[4]);
	static void ShowUI();
	static std::string ConfigString();
};