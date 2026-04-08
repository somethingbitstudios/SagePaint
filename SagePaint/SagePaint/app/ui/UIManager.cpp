#include "UIManager.h"
#include <imgui.h>
#include "../tools/ToolManager.h"
#include "../CanvasManager.h"
#include <algorithm>
#include "../ProjectManager.h"

CursorFocus UIManager::cursor_focus = FOCUS_CANVAS;
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
	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y - 18));

	ImGui::Begin("LeftMenu", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("Tools:");
	if (ImGui::Button("Pencil"))
		ToolManager::SetTool(TOOL_PENCIL);
	if (ImGui::Button("Line"))
		ToolManager::SetTool(TOOL_LINE);
	if (ImGui::Button("Shape"))
		ToolManager::SetTool(TOOL_SHAPE);
	if (ImGui::Button("Fill"))
		ToolManager::SetTool(TOOL_FILL);
	if (ImGui::Button("Select"))
		ToolManager::SetTool(TOOL_SELECT);
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 250, 18));
	ImGui::SetNextWindowSize(ImVec2(250, ImGui::GetIO().DisplaySize.y - 18));
	ImGui::Begin("RightMenu", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("The right menu");
	ImGui::ColorEdit4("My Color", (float*)&CanvasManager::color);
	ImGui::ColorEdit4("Background", (float*)&CanvasManager::backgroundColor);


	ImGui::Text("Layers:");
	for (int i = CanvasManager::obj->layers->size() - 1; i >= 0; i--) {
		ImGui::PushID(i);

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


		if (CanvasManager::obj->selectedLayer == i) {

			ImGui::SameLine(); ImGui::Text("*");
		}

		if (ImGui::InputFloat("", &(*CanvasManager::obj->layers)[i]->opacity))
		{
			(*CanvasManager::obj->layers)[i]->opacity = std::max(0.0f, std::min(1.0f, (*CanvasManager::obj->layers)[i]->opacity));

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
