#include "ToolManager.h"
#include <imgui.h>
#include "../ui/Icons.h"
#include "PencilTool.h"
#include "LineTool.h"
#include "ShapeTool.h"
#include "SelectTool.h"
#include "FillTool.h"
#include "../CanvasManager.h"
#include "PickerTool.h"
std::vector<ImTextureID> icons;

TOOL_TYPE ToolManager::tool_type = TOOL_PENCIL;
void ToolManager::SetTool(TOOL_TYPE type) {
	tool_type = type;
	//TODO: possibly initiation of tool, like PencilTool::Init()?
} 
void ToolManager::Init() {
	Icons::AddIcon("./icons/tool_pencil40.png",icons);
	Icons::AddIcon("./icons/tool_line40.png", icons);
	Icons::AddIcon("./icons/tool_shape40.png", icons);
	Icons::AddIcon("./icons/tool_fill40.png", icons);
	Icons::AddIcon("./icons/tool_select40.png", icons);
	Icons::AddIcon("./icons/tool_picker40.png", icons);
}
void ToolManager::ShowUI() {
	ImGui::Text("Tools:");
	if (ImGui::ImageButton(
		"SetToPencilButton",
		icons[0],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		SetTool(TOOL_PENCIL);
	}
	ImGui::SameLine();
	if (ImGui::ImageButton(
		"SetToLineButton",
		icons[1],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		SetTool(TOOL_LINE);
	}
	ImGui::SameLine();
	if (ImGui::ImageButton(
		"SetToShapeButton",
		icons[2],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		SetTool(TOOL_SHAPE);
	}
	ImGui::SameLine();
	if (ImGui::ImageButton(
		"SetToFillButton",
		icons[3],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		SetTool(TOOL_FILL);
	}
	if (ImGui::ImageButton(
		"SetToSelectButton",
		icons[4],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		SetTool(TOOL_SELECT);
	}
	ImGui::SameLine();
	if (ImGui::ImageButton(
		"SetToPickerButton",
		icons[5],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		SetTool(TOOL_PICKER);
	}

	switch (tool_type) {
	case TOOL_PENCIL:
		PencilTool::ShowUI();
		break;
	case TOOL_LINE:
		LineTool::ShowUI();
		break;
	case TOOL_FILL:
		FillTool::ShowUI();
		break;
	case TOOL_SHAPE:
		ShapeTool::ShowUI();
		break;
	case TOOL_SELECT:
		SelectTool::ShowUI();
		break;	
	case TOOL_PICKER:
		PickerTool::ShowUI();
		break;
	}

	ImGui::Separator();
	if (ImGui::Button(CanvasManager::erase ? "Stop erasing" : "Start erasing")) {
		CanvasManager::erase = !CanvasManager::erase;
	}
}