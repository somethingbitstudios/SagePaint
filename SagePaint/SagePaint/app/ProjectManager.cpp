#include "ProjectManager.h"
#include <debug.h>
#include <imgui.h>

#include "imgui.h"
#include "imgui_stdlib.h" 
#include "CanvasManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
bool ProjectManager::dirty = true;
std::string ProjectManager::name = "test";
bool ProjectManager::ShowFileUI()
{
	static bool show_exit_popup = false;
	static bool show_save_popup = false;
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New", "Shortcut")) {

		}
		if (ImGui::MenuItem("Open", "Shortcut")) {

		}
		if (ImGui::MenuItem("Save", "Shortcut")) {
			
		}
		if (ImGui::MenuItem("Save As", "Shortcut")) {
			if (dirty) {
				show_save_popup = true;
			}
		}
		if (ImGui::MenuItem("Exit")) {
			//Open dialog box	
			if (dirty) {
				show_exit_popup = true;
			}
			else {
				ImGui::EndMenu();
				return false;
			}
			//runApp = false;
		}


		ImGui::EndMenu();
	}
		if (show_exit_popup)
		{
			ImGui::OpenPopup("Save Changes?");
			show_exit_popup = false; 
		}

		if (show_save_popup) {
			ImGui::OpenPopup("Save As");
			show_save_popup = false;
		}
		//SAVEAS
		
		static std::string save_filename = "C:\\temp\\new.sagepaint";


		if (ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter file name or path:");

		
			ImGui::InputText("##save_filename", &save_filename);

			ImGui::Separator();

			if (ImGui::Button("Save", ImVec2(120, 0)))
			{
				// Pass the string to your Save function
				Save(save_filename);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		//EXIT
		if (ImGui::BeginPopupModal("Save Changes?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Save before quitting?");
			ImGui::Separator();

			
			if (ImGui::Button("Save", ImVec2(120, 0))) {
				
				show_save_popup = true;
				ImGui::CloseCurrentPopup();
				
			}
			ImGui::SameLine();
			if (ImGui::Button("Save As", ImVec2(120, 0))) {

				show_save_popup = true;
				ImGui::CloseCurrentPopup();

			}
			if (ImGui::Button("Quit", ImVec2(120, 0))) {

				ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
				return false;
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup(); 
			}

			ImGui::EndPopup();
		}

		return true;
}



bool ProjectManager::Exit()
{
	ImGui::OpenPopup("Hey");
		if(ImGui::Button("Close")){
			
			ImGui::EndPopup();
			return true;
		}
	

	
	return false;
}

bool ProjectManager::Save(std::string path) //path may be folder, or a file to override. only override .sagepaint files, otherwise
{
	/*
	//.sagepaint structure: basically a file archive for bundling
	//project.conf (json)- has project configuration and info (resolution , time made, description etc)
	//also has layer data:
	number of layers
	N times this:
	{
	Layer Name on one row
	opacity float
	...
	imagedata: filepath (layerdata/x.png)
	anything, layer mask for example: filepath
	...
	}

	layerdata/ folder
	inside, .png files, by default, name [layer index].png, ex.: 0.png
	then layer mask can be 0_mask_0.png etc.
	*/

	//save project name (std::string name is in thecpp file)

	
		namespace fs = std::filesystem;
	fs::path targetPath(path);

	DLOG(targetPath);


	fs::create_directories(targetPath);
	fs::path dataDir = targetPath / "data";
	fs::create_directories(dataDir);

	fs::path configFile = targetPath / "project.conf";
	std::ofstream out(configFile);

	if (!out) {
		DLOG("Failed to create config file")
		return 1;
	}

	out << "{\n";
	out << "	name: \"";
	out << name;
	out << "\"\n	version: \"";
	out << "0.64";
	out << "\"\n	description: \"";
	out << "This is a SagePaint project!";

	out << "\"\n	layers: {";
	//for
	unsigned int layerCount = CanvasManager::obj->layers->size();
	for (int i = 1; i < layerCount; i++) {//WARN: 1 because of preview
		LayerPtr layer = (*CanvasManager::obj->layers)[i];
		out << "\n		{";
		out << "\n			id: ";
		out << layer->id;
		out << "\n			name: \"";
		out << layer->name;
		out << "\"\n			visible: ";
		out << layer->visible;
		out << "\n			opacity: ";
		out << layer->opacity;
		out << "\n			blend: ";
		out << (int)layer->blend;
		out << "\n		},";
		//put .png into data
	}
	out << "\n	}";

	//layers
	out << "\n}\n";

	out.close();

	
	return false;
}

bool ProjectManager::SaveAs()
{
	return false;
}

bool ProjectManager::Open()
{
	return false;
}

bool ProjectManager::New()
{
	return false;
}
