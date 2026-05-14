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
	
	FillTool::FillRender(image->texture, image->width, image->height, upPos.x,upPos.y,color,mode);
	
	CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);
    ProjectManager::Dirty();

}
void FillTool::FillRender(unsigned char* texture, int w, int h, int x, int y, unsigned char color[4],FillMode fm) {
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

    float color3 = (color[3] / 255.0f) * opacity;
    float invopac = (1 - color3);

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
      
        float alpha2 = (texture[idx+3] / 255.0f) * invopac;
        float alpha3 = color3 + alpha2;
        unsigned char old[] = { texture[idx],texture[idx+1],texture[idx+2],texture[idx + 3] };
        if (alpha3 > 0.002) {
            texture[idx] = round((color[0] * color3 + texture[idx] * alpha2) / alpha3);
            texture[idx + 1] = round((color[1] * color3 + texture[idx + 1] * alpha2) / alpha3);
            texture[idx + 2] = round((color[2] * color3 + texture[idx + 2] * alpha2) / alpha3);
            texture[idx+3] = alpha3 * 255;


        }
        else {
            texture[idx] = 0;
            texture[idx + 1] = 0;
            texture[idx + 2] = 0;
            texture[idx + 3] = 0;
        }
        return !(old[0] == texture[idx] && old[1] == texture[idx + 1] && old[2] == texture[idx + 2] && old[3] == texture[idx + 3]);
        /*
        texture[idx] = color[0];
        texture[idx + 1] = color[1];
        texture[idx + 2] = color[2];
        texture[idx + 3] = color[3];
        */
        };

    switch (fm) {
    case FILL_4WAY: {
        std::queue<std::pair<int, int>> q;

        setPixelColor(x, y);
        q.push({ x, y });

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();


            //left
            if (cx > 0 && isTargetColor(cx - 1, cy)) {
                bool result = setPixelColor(cx - 1, cy);
                if (result) q.push({ cx - 1, cy });
            }
            //right
            if (cx < w - 1 && isTargetColor(cx + 1, cy)) {
                bool result = setPixelColor(cx + 1, cy);
                if (result) q.push({ cx + 1, cy });
            }
            //up
            if (cy > 0 && isTargetColor(cx, cy - 1)) {
                bool result = setPixelColor(cx, cy - 1);
                if (result) q.push({ cx, cy - 1 });
            }
            //down
            if (cy < h - 1 && isTargetColor(cx, cy + 1)) {
                bool result = setPixelColor(cx, cy + 1);
                if (result) q.push({ cx, cy + 1 });
            }
        }
        break;
    }
    case FILL_SCANLINE: {
        // Wrapper for bounds and target color checking
        auto Inside = [&](int px, int py) {
            if (px < 0 || px >= w || py < 0 || py >= h) return false;
            return isTargetColor(px, py);
            };

        if (!Inside(x, y)) break;

        // Vector used as a LIFO stack for the scanline spans
        std::vector<std::pair<int, int>> s;
        s.push_back({ x, y });

        auto scan = [&](int lx, int rx, int py) {
            if (py < 0 || py >= h) return;
            bool span_added = false;
            for (int px = lx; px <= rx; ++px) {
                if (!Inside(px, py)) {
                    span_added = false;
                }
                else if (!span_added) {
                    s.push_back({ px, py });
                    span_added = true;
                }
            }
            };

        while (!s.empty()) {
            auto [cx, cy] = s.back();
            s.pop_back();

            int lx = cx;
            while (Inside(lx - 1, cy)) {
                setPixelColor(lx - 1, cy);
                lx = lx - 1;
            }

            int current_x = cx;
            while (Inside(current_x, cy)) {
                setPixelColor(current_x, cy);
                current_x = current_x + 1;
            }

            // 'current_x - 1' is the rightmost pixel of the current span
            scan(lx, current_x - 1, cy + 1);
            scan(lx, current_x - 1, cy - 1);
        }
        break;
    }
        }
   
}
float FillTool::opacity = 1;

FillMode FillTool::mode = FILL_4WAY;
void FillTool::ShowUI() {
    ImGui::Separator();
    ImGui::Text("Fill Settings:");
    if (ImGui::InputFloat("opacity", &opacity, 0.1f, 0.2f, "%.2f")) {
        opacity = std::max(0.0f, std::min(1.0f, opacity));
    }
    const char* fill_modes[] = { "Scanline", "4way" };
    static int temp_mode = (int)mode;
    if (ImGui::Combo("Mode", &(temp_mode), fill_modes, IM_ARRAYSIZE(fill_modes))) {

        mode = (FillMode)temp_mode;
    }

}
std::string FillTool::ConfigString()
{
    std::stringstream ss;

    ss << R"(	{
		"mode": )" << "0"/*static_cast<int>(mode)*/ << R"(
	})";

    return ss.str();
}
void FillTool::LoadConfig(const nlohmann::json& j) {

 
    if (j.contains("mode")) {
       // mode = (PencilMode)j["mode"].get<int>();
    }
}