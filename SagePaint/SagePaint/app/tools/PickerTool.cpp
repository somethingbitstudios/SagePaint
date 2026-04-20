#include "PickerTool.h"
#include <imgui.h>
#include "../CanvasManager.h"
PickerMode PickerTool::mode = SINGLELAYER;
glm::ivec2 PickerTool::downPos = { -1,-1 };
void PickerTool::Pick() {
	downPos = CanvasManager::GetRelativeCursorPos();
	ImagePtr i = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;
	if (downPos.x >= 0 && downPos.x < i->width && downPos.y >= 0 && downPos.y < i->height) {
		unsigned char* clr = &(i->texture[4 * (downPos.x + downPos.y * i->width)]);

		CanvasManager::color[0] = clr[0];
		CanvasManager::color[1] = clr[1];
		CanvasManager::color[2] = clr[2];
		CanvasManager::color[3] = clr[3];

		CanvasManager::colorFloat[0] = clr[0] / 255.0f;
		CanvasManager::colorFloat[1] = clr[1] / 255.0f;
		CanvasManager::colorFloat[2] = clr[2] / 255.0f;
		CanvasManager::colorFloat[3] = clr[3] / 255.0f;

	}

	}

void PickerTool::ShowUI() {
	ImGui::Separator();
	ImGui::Text("Picker Settings:");
	
	const char* picker_modes[] = { "Single Layer", "Final Composite" };
	static int temp_mode = (int)mode;
	if (ImGui::Combo("Mode", &(temp_mode), picker_modes, IM_ARRAYSIZE(picker_modes))) {

		mode = (PickerMode)temp_mode;
	}
}