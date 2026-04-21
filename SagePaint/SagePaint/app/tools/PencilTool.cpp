#include "PencilTool.h"
#include "../CanvasManager.h"
#include "../file/Layer.h"
#include "LineTool.h"
#include "../ProjectManager.h"
#include <imgui.h>
#include <algorithm>
#include <sstream>
float PencilTool::opacity = 1;
//TODO: make this at least connect using the line tool
void PencilTool::Stroke() {
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	if (mode == PENCIL_NORMAL) {
		unsigned char *color;
		
		
		
		if (CanvasManager::erase) {
			color = CanvasManager::erasePreviewColor;
			//TODO:override paint on top mode to always be replace color mode!
		}
		else {
			color = CanvasManager::color;
		}
		//TODO: support width
		//TODO: optimize
		//if (color[3] == 0)return;
		glm::ivec2 lastPos = CanvasManager::GetLastRelativeCursorPos();
		glm::ivec2 pos = CanvasManager::GetRelativeCursorPos();
		CanvasObjectPtr obj = CanvasManager::GetCanvas();

		ImagePtr image = (*obj->layers)[0]->image;//WARN:hardcoded!
		LineTool::LineRender(image->texture, image->width, image->height, lastPos.x, lastPos.y, pos.x, pos.y,strokeSize, color,1);
		CanvasManager::obj->Changed(0);
	}
	else if (mode == PENCIL_SIMPLE) {
		float* color_float = CanvasManager::colorFloat;

		int color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
		int radius = strokeSize;

		glm::ivec2 pos = CanvasManager::GetRelativeCursorPos();
		int start_x = pos.x - radius; int end_x = pos.x + radius;
		int start_y = pos.y - radius; int end_y = pos.y + radius;
		//don't draw if it's not even on the canvas
		ImagePtr image = (*(CanvasManager::obj->layers))[CanvasManager::obj->selectedLayer]->image;//WARN:hardcoded!

		if (end_x < 0 || end_y < 0 || start_x >= image->width || start_y >= image->height) {
			return;
		}
		//not sure if optimal, meh
		start_x = std::max(start_x, 0);
		start_y = std::max(start_y, 0);
		end_x = std::min(end_x, image->width - 1);
		end_y = std::min(end_y, image->height - 1);
		int difference_x = end_x - start_x;
		unsigned char color2[] = { color[0] * opacity,color[1] * opacity,color[2] * opacity,color[3] * opacity };
		float invopac = (1 - opacity);
		for (int i = start_y; i <= end_y; i++) {
			unsigned char* img = &image->texture[(start_x + i * image->width) * 4]; //should be faster than it being in the for bellow
			for (int j = 0; j <= difference_x; j++) {

				img[0] = color2[0]+img[0]*invopac;
				img[1] = color2[1] + img[1] * invopac;
				img[2] = color2[2] + img[2] * invopac;
				img[3] = color2[3] + img[3] * invopac;
				img += 4;



			}
		}
		CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);
	}
	/*
	//TODO: optimize
	float* color_float = CanvasManager::color;

	int color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	int radius = 16;

	
	int start_x = pos.x - radius; int end_x = pos.x + radius;
	int start_y = pos.y - radius; int end_y = pos.y + radius;
	//don't draw if it's not even on the canvas
	if (obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*obj->layers)[obj->selectedLayer]->image;//WARN:hardcoded!

	if (end_x < 0 || end_y < 0 || start_x >= image->width || start_y >= image->height) {
		return;
	}
	//not sure if optimal, meh
	start_x = std::max(start_x, 0);
	start_y = std::max(start_y, 0);
	end_x = std::min(end_x, image->width-1);
	end_y = std::min(end_y, image->height-1);
	int difference_x = end_x - start_x;
	for (int i = start_y; i <= end_y; i++) {
		unsigned char* img = &image->texture[(start_x + i * image->width)*4]; //should be faster than it being in the for bellow
		for (int j = 0; j <= difference_x; j++) {
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
			img += 4;
				

			
		}
	}
	*/
	
	

}
void PencilTool :: StrokeStart() {
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert

	(*CanvasManager::obj->layers)[0]->opacity = opacity * (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->opacity;
}
void PencilTool::StrokeEnd() {
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert

	ImagePtr p = (*CanvasManager::obj->layers)[0]->image;
	ImagePtr j = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;
	if (CanvasManager::erase) {
		j->ClearOverlay(p->texture, p->width, p->height, 0, 0, p->width, p->height, 0, 0, opacity);
	}
	else {
		j->CopyOverlay(p->texture, p->width, p->height, 0, 0, p->width, p->height, 0, 0, opacity);

	}
	//	j->CopyOverlay(p->texture, p->width, p->height, 0, 0, p->width, p->height, 0, 0,opacity);
	p->Clear(0, 0, p->width, p->height);
	//copy 
	(*CanvasManager::obj->layers)[0]->opacity = 0.5f;
	CanvasManager::obj->Changed(0);
	CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);

	ProjectManager::Dirty();
}
float PencilTool::strokeSize = 1.0f;
PencilMode PencilTool::mode = PENCIL_NORMAL;

void PencilTool::ShowUI() {
	ImGui::Separator();
	ImGui::Text("Pencil Settings:");
	if (ImGui::InputFloat("opacity", &PencilTool::opacity, 0.1f, 0.2f, "%.2f")) {
		opacity = std::max(0.0f,std::min(1.0f, opacity));
	}
	if (ImGui::InputFloat("size", &PencilTool::strokeSize, 1, 2, "%.1f")) {
		strokeSize =std::max(0.5f, strokeSize);
	}
	const char* pencil_modes[] = { "Normal", "Simple"};
	static int temp_mode = (int) mode;
	if (ImGui::Combo("Mode", &(temp_mode), pencil_modes, IM_ARRAYSIZE(pencil_modes))) {

		mode = (PencilMode)temp_mode;
	}
	
}

std::string PencilTool::ConfigString()
{
	std::stringstream ss;

	ss << R"(	{
		"strokeSize": )" << strokeSize << R"(,
		"mode": )" << static_cast<int>(mode) << R"(
	})";

	return ss.str();
}

void PencilTool::LoadConfig(const nlohmann::json& j) {

	if (j.contains("strokeSize")) {
		strokeSize = j["strokeSize"].get<float>();
	}
	if (j.contains("mode")) {
		mode = (PencilMode)j["mode"].get<int>();
	}
}