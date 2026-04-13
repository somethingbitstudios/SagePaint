#include "UIManager.h"
#include <imgui.h>
#include "../tools/ToolManager.h"
#include "../CanvasManager.h"
#include <algorithm>
#include "../ProjectManager.h"

CursorFocus UIManager::cursor_focus = FOCUS_CANVAS;
std::vector<ImTextureID> icons;
void AddIcon(std::string path) {
	GLuint texId;
	ImagePtr image = std::make_shared<Image>(path);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image->texture);

	icons.push_back((void*)(intptr_t)texId);
}
void UIManager::Init() {
	AddIcon("./icons/tool_pencil40.png");
	AddIcon("./icons/tool_line40.png");
	AddIcon("./icons/tool_shape40.png");
	AddIcon("./icons/tool_fill40.png");
	AddIcon("./icons/tool_select40.png");
}
void UIManager::SetCursorFocus(CursorFocus c)
{
	cursor_focus = c;
}

CursorFocus UIManager::GetCursorFocus()
{
	return cursor_focus;
}


bool UIManager::ShowUI()
{
	if (ImGui::GetIO().WantCaptureMouse) {
		SetCursorFocus(FOCUS_UI);
	}
	else {
		SetCursorFocus(FOCUS_CANVAS);//WARN:activates even outside the window, might be an issue, might not be
	}

	//show the ui

	ImGui::SetNextWindowPos(ImVec2(0, 18));
	ImGui::SetNextWindowSize(ImVec2(232, ImGui::GetIO().DisplaySize.y - 18));

	ImGui::Begin("LeftMenu", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("Tools:");
	if (ImGui::ImageButton(
		"SetToPencilButton",
		icons[0],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		ToolManager::SetTool(TOOL_PENCIL);
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
		ToolManager::SetTool(TOOL_LINE);
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
		ToolManager::SetTool(TOOL_SHAPE);
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
		ToolManager::SetTool(TOOL_FILL);
	}
	if (ImGui::ImageButton(
		"SetToSelectButton",
		icons[4],
		ImVec2(40, 40),			//size
		ImVec2(0.0f, 0.0f),     //uv0
		ImVec2(1.0f, 1.0f),     //uv1
		ImVec4(0.1, 0.1, 0.1, 1)//bg color
	)) {
		ToolManager::SetTool(TOOL_SELECT);
	}

	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 18));
	ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 18));
	ImGui::Begin("RightMenu", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("The right menu");
	ImGui::ColorEdit4("My Color", (float*)&CanvasManager::color);
	ImGui::ColorEdit4("Background", (float*)&CanvasManager::backgroundColor);


	ImGui::Text("Layers:");
	for (int i = CanvasManager::obj->layers->size() - 1; i >= 0; i--) {
		ImGui::PushID(i);

		if (CanvasManager::obj->selectedLayer == i) {
			// Get the current screen position and available width
			ImVec2 cursor = ImGui::GetCursorScreenPos();
			ImVec2 p_min = ImVec2(cursor.x-10,cursor.y-3);
			ImVec2 p_max = ImVec2(p_min.x + ImGui::GetContentRegionAvail().x, p_min.y + ImGui::GetFrameHeight()*2+10);

			// Draw a grey rectangle (RGBA: 128, 128, 128, 255)
			ImU32 bg_color = IM_COL32(255, 0, 255, 32);
			ImGui::GetWindowDrawList()->AddRectFilled(p_min, p_max, bg_color);
		}

		if (ImGui::Button((*CanvasManager::obj->layers)[i]->name.c_str()))
		{
			CanvasManager::obj->selectedLayer = i;
			CanvasManager::obj->model->selected_layer = i;//TODO: wrap

		}

		ImGui::SameLine();

		if (ImGui::Button(CanvasManager::obj->GetVisible(i) ? "Hide" : "Show"))
		{
			CanvasManager::obj->ToggleVisible(i);

		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(56);
		if (ImGui::InputFloat("", &(*CanvasManager::obj->layers)[i]->opacity))
		{
			CanvasManager::OpacityChanged(i);
			//(*CanvasManager::obj->layers)[i]->opacity = std::max(0.0f, std::min(1.0f, (*CanvasManager::obj->layers)[i]->opacity));

		}
		ImGui::SameLine();
		if (ImGui::Button("X")) {
			//TODO: schedule instead of the break hotfix
			CanvasManager::obj->Remove(i);
			ImGui::PopID();
			break;
		}


		ImGui::SameLine();

		if (ImGui::Button("Up"))
		{
			CanvasManager::obj->SwapLayerUp(i);
		}

		ImGui::SameLine();

		if (ImGui::Button("Down")) {
			CanvasManager::obj->SwapLayerDown(i);

		}

		const char* blend_modes[] = { "Normal", "Darken", "Lighten" };

		if (ImGui::Combo("Blend", &((*CanvasManager::obj->layers)[i]->blendCandidate), blend_modes, IM_ARRAYSIZE(blend_modes))) {

			CanvasManager::ChangeBlendMode(i);
		}

		
		ImGui::PopID();
	}
	if (ImGui::Button("New Layer")) {
		CanvasManager::obj->AddLayer();
	}

	ImGui::End();


	if (ImGui::BeginMainMenuBar())
	{

		if (!ProjectManager::ShowFileUI()) {

			ImGui::EndMainMenuBar();
			return false;
		}


		if (ImGui::BeginMenu("View"))
		{

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}
	return true;



}
