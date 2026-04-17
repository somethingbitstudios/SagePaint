#include "UIManager.h"
#include <imgui.h>
#include "../tools/ToolManager.h"
#include "../CanvasManager.h"
#include <algorithm>
#include "../ProjectManager.h"
#include "Icons.h"
#include "../SettingsManager.h"

CursorFocus UIManager::cursor_focus = FOCUS_CANVAS;


void UIManager::Init() {
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

	
	ToolManager::ShowUI();
	ImGui::End();


	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 18));
	ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 18));
	ImGui::Begin("RightMenu", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

	ImGui::Text("The right menu");
	if (ImGui::ColorEdit4("My Color", (float*)&CanvasManager::colorFloat)) {
		CanvasManager::UpdateColor();
	}
	

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

		const char* blend_modes[] = { "Normal", "Darken", "Lighten","Add","Multiply"};

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
			if (ImGui::MenuItem("Settings", SettingsManager::showWindow?"(hide)":"(show)")) {
				//open settings window  by changing SettingsManager::showWindow to true
				SettingsManager::showWindow = true;
			}
			
			ImGui::EndMenu();
		}
		

		ImGui::EndMainMenuBar();
	}
	SettingsManager::ShowUI();//write the inside of this function
	return true;



}
