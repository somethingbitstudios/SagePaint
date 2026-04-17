#include "FillTool.h"
#include <debug.h>
#include <glm/ext/vector_int2.hpp>
#include "../CanvasManager.h"
#include <queue>
#include "../ProjectManager.h"

#include <sstream>
#include <imgui.h>
//TODO: make it, make multiple versions too
void FillTool::Fill() {
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();

	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
    /*
	float* color_float = CanvasManager::colorFloat;
	//TODO: support width
	//TODO: optimize
	unsigned char color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	if (color[3] == 0)return;
    */

    unsigned char* color;
    if (CanvasManager::erase) {
        color = CanvasManager::transparent;
        //TODO:override paint on top mode to always be replace color mode!
    }
    else {
        color = CanvasManager::color;
    }
	ImagePtr image = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;//WARN:hardcoded!
	
	FillTool::FillRender(image->texture, image->width, image->height, upPos.x,upPos.y,color);
	
	CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);
    ProjectManager::Dirty();

}
void FillTool::FillRender(unsigned char* texture, int w, int h, int x, int y, unsigned char color[4]) {
    if (w <= 0 || h <= 0 || x < 0 || x >= w || y < 0 || y >= h) return;
    int startIdx = (y * w + x) * 4;

    //target color
    unsigned char startColor[4] = {
        texture[startIdx],
        texture[startIdx + 1],
        texture[startIdx + 2],
        texture[startIdx + 3]
    };

    if (startColor[0] == color[0] && startColor[1] == color[1] &&
        startColor[2] == color[2] && startColor[3] == color[3]) {
        return;
    }
    //maybe I'll convert to inline function, but lambdas might be better here for now at least
    auto isTargetColor = [&](int px, int py) {
        int idx = (py * w + px) * 4;
        return (texture[idx] == startColor[0] &&
            texture[idx + 1] == startColor[1] &&
            texture[idx + 2] == startColor[2] &&
            texture[idx + 3] == startColor[3]);
        };

    auto setPixelColor = [&](int px, int py) {
        int idx = (py * w + px) * 4;
        texture[idx] = color[0];
        texture[idx + 1] = color[1];
        texture[idx + 2] = color[2];
        texture[idx + 3] = color[3];
        };

    
    std::queue<std::pair<int, int>> q;

    setPixelColor(x, y);
    q.push({ x, y });

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        
        //left
        if (cx > 0 && isTargetColor(cx - 1, cy)) {
            setPixelColor(cx - 1, cy);
            q.push({ cx - 1, cy });
        }
        //right
        if (cx < w - 1 && isTargetColor(cx + 1, cy)) {
            setPixelColor(cx + 1, cy);
            q.push({ cx + 1, cy });
        }
        //up
        if (cy > 0 && isTargetColor(cx, cy - 1)) {
            setPixelColor(cx, cy - 1);
            q.push({ cx, cy - 1 });
        }
        //down
        if (cy < h - 1 && isTargetColor(cx, cy + 1)) {
            setPixelColor(cx, cy + 1);
            q.push({ cx, cy + 1 });
        }
    }
}

void FillTool::ShowUI() {
    ImGui::Separator();
    ImGui::Text("Fill Settings:");
  

}
std::string FillTool::ConfigString()
{
    std::stringstream ss;

    ss << R"(	{
		"mode": )" << "0"/*static_cast<int>(mode)*/ << R"(
	})";

    return ss.str();
}
