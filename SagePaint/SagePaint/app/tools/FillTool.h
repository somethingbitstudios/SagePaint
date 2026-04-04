#pragma once

class FillTool {
public:
	static void Fill();
	static void FillRender(unsigned char* texture, int w, int h, int x, int y, unsigned char color[4]);
};