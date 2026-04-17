#include "SelectTool.h"
#include "../CanvasManager.h"
#include "ShapeTool.h"
#include "../ProjectManager.h"
#include <sstream>
#include <imgui.h>
glm::ivec2 lastUpPos2 = { 0,0 };
glm::ivec2 SelectTool::downPos = { 0,0 };
ImagePtr selectedAreaImage;
bool SelectTool::dragMode = false;

void SelectTool::SelectStart()
{
	downPos = CanvasManager::GetRelativeCursorPos();
	lastUpPos2 = downPos;
}

void SelectTool::SelectPreview()
{
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	ShapeTool::ShapeRenderFilledRect(image->texture, image->width, image->height, downPos, lastUpPos2, CanvasManager::transparent);

	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();
	float* color_float = CanvasManager::colorFloat;
	//TODO: support width
	//TODO: optimize
	unsigned char color[4] = { 0,0,255,255 };//make this only happen once per color setting
	if (color[3] == 0)return;

	ShapeTool::ShapeRenderFilledRect(image->texture, image->width, image->height, downPos, upPos, color);

	lastUpPos2 = upPos;
	CanvasManager::obj->Changed(0);

}

void SelectTool::SelectEnd()
{
	//remove afterimage from preview
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	ShapeTool::ShapeRenderFilledRect(image->texture, image->width, image->height, downPos, lastUpPos2, CanvasManager::transparent);
	//CanvasManager::obj->Changed(0);

	int x_min = std::min(downPos.x, lastUpPos2.x);
	int y_min = std::min(downPos.y, lastUpPos2.y);
	//take the selected data,put it in a texture
	int w = abs(downPos.x - lastUpPos2.x)+1;
	int h = abs(downPos.y - lastUpPos2.y)+1;
	//if (w > 0 && h > 0) {
	selectedAreaImage = std::make_shared<Image>(w, h);

	ImagePtr j = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;
	selectedAreaImage->Copy(j->texture, j->width, j->height, x_min, y_min, w, h, 0, 0);

		//now copy the selected to the preview each move TODO this
	//image->Copy(selectedAreaImage->texture, selectedAreaImage->width, selectedAreaImage->height, 0, 0, w, h, x_min, y_min);
		//show the anchor points using opengl, not software rendering
		
	//}
	SelectDragRender();
	dragMode = true;
	//CanvasManager::obj->Changed(0);
	
}

void SelectTool::SelectDragRender()
{
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();

	//if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!
	image->Clear(lastUpPos2.x, lastUpPos2.y, selectedAreaImage->width, selectedAreaImage->height);
	image->Copy(selectedAreaImage->texture, selectedAreaImage->width, selectedAreaImage->height, 0, 0, -1, -1, upPos.x, upPos.y);
	CanvasManager::obj->Changed(0);
	lastUpPos2 = upPos;

}

void SelectTool::SelectDragCommit()
{
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();

	//if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!
	image->Clear(lastUpPos2.x, lastUpPos2.y, selectedAreaImage->width, selectedAreaImage->height);
	CanvasManager::obj->Changed(0);
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	image = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;//WARN:hardcoded!

	image->Copy(selectedAreaImage->texture, selectedAreaImage->width, selectedAreaImage->height, 0, 0, -1, -1, upPos.x, upPos.y);
	
	CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);
	//lastUpPos2 = upPos;
	dragMode = false;//simple version for debugging
	ProjectManager::Dirty();
}

void SelectTool::ShowUI() {
	ImGui::Separator();
	ImGui::Text("Select Settings:");
	/*if (ImGui::InputFloat("size", &PencilTool::strokeSize, 1, 2, "%.1f")) {
		strokeSize = std::max(0.5f, strokeSize);
	}
	const char* pencil_modes[] = { "Normal", "Simple" };
	static int temp_mode = 0;
	if (ImGui::Combo("Mode", &(temp_mode), pencil_modes, IM_ARRAYSIZE(pencil_modes))) {

		mode = (PencilMode)temp_mode;
	}
	*/
}

std::string SelectTool::ConfigString()
{
	std::stringstream ss;

	ss << R"(	{
		"mode": )" << static_cast<int>(0) << R"(
	})";

	return ss.str();
}




