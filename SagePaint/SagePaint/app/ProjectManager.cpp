#include "ProjectManager.h"
#include <debug.h>
#include <imgui.h>

#include "imgui.h"
#include "imgui_stdlib.h" 
#include "CanvasManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "stb_image.h"
#include <nlohmann/json.hpp>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
bool ProjectManager::dirty = true;
std::string ProjectManager::name = "test";
bool ProjectManager::ShowFileUI()
{
	static bool show_exit_popup = false;
	static bool show_save_popup = false;
	static bool show_open_popup = false;
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New", "Shortcut")) {
			New();//if dirty, dont do this
		}
		if (ImGui::MenuItem("Open", "Shortcut")) {
			if (false/*dirty*/) {
				show_save_popup = true;
			}
			else {
				show_open_popup = true;
			}
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
			if (false/*dirty*/) {
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
		if (show_open_popup) {
			ImGui::OpenPopup("Open project");
			show_open_popup = false;
		}
		//SAVEAS

		static std::string open_filename = "C:\\temp\\new.sagepaint";


		if (ImGui::BeginPopupModal("Open project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter file name or path to open:");


			ImGui::InputText("##open_filename", &open_filename);

			ImGui::Separator();

			if (ImGui::Button("Open", ImVec2(120, 0)))
			{
				// Pass the string to your Save function
				Open(open_filename);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
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
	fs::path dataDir = targetPath / "data" / "";
	fs::create_directories(dataDir);

	fs::path configFile = targetPath / "project.conf";
	std::ofstream out(configFile);

	if (!out) {
		DLOG("Failed to create config file")
		return 1;
	}

	out << R"({
    "name": ")" << name << R"(",
    "version": "0.64",
    "description": "This is a SagePaint project!",
	"resolutionX": )" << CanvasManager::obj->resX << R"(,
	"resolutionY": )" << CanvasManager::obj->resY;

	int layerCount = CanvasManager::obj->layers->size();
	if (layerCount > 0) {
		out << ",\n		\"layers\": [";

		int layerCount = CanvasManager::obj->layers->size();
		for (int i = 1; i < layerCount - 1; i++) {//WARN: 1 because of preview
			LayerPtr layer = (*CanvasManager::obj->layers)[i];
			out << "\n		{";
			out << "\n			\"id\": ";
			out << layer->id;
			out << ",\n			\"name\": \"";
			out << layer->name;
			out << "\",\n			\"visible\": ";
			out << ((layer->visible) ? "true" : "false");
			out << ",\n			\"opacity\": ";
			out << std::fixed << layer->opacity;
			out << ",\n			\"blend\": ";
			out << (int)layer->blend;
			out << "\n		},";
			//put .png into data
			ImagePtr img = layer->image;

			std::string gg = dataDir.string() + std::to_string(layer->id) + ".png";
			stbi_write_png(gg.c_str(), img->width, img->height, 4, img->texture, img->width * 4);
		}

		LayerPtr layer = (*CanvasManager::obj->layers)[layerCount - 1];
		out << "\n		{";
		out << "\n			\"id\": ";
		out << layer->id;
		out << ",\n			\"name\": \"";
		out << layer->name;
		out << "\",\n			\"visible\": ";
		out << ((layer->visible) ? "true" : "false");
		out << ",\n			\"opacity\": ";
		out << std::fixed << layer->opacity;
		out << ",\n			\"blend\": ";
		out << (int)layer->blend;
		out << "\n		}";
		//put .png into data
		ImagePtr img = layer->image;

		std::string gg = dataDir.string() + std::to_string(layer->id) + ".png";
		stbi_write_png(gg.c_str(), img->width, img->height, 4, img->texture, img->width * 4);


		out << "\n	]";
	}
	

	//layers
	out << "\n}\n";

	out.close();

	//Open(path);//Test
	return false;
}



bool ProjectManager::Open(std::string path)
{
	Clear();//clear data


	namespace fs = std::filesystem;
	fs::path targetPath(path);

	fs::path dataDir = targetPath / "data" / "";
	fs::path configFile = targetPath / "project.conf";
	
	
	using json = nlohmann::json;

	std::ifstream in(configFile);
	if (!in) {
		DLOG("Failed to load config file");
		return 1;
	}

	json j;
	in >> j;

	std::string name = j.value("name", "");
	std::string version = j.value("version", "");
	std::string description = j.value("description", "");
	unsigned int rX, rY;
	rX = j.value("resolutionX", 640);
	rY = j.value("resolutionY",400);
	//validate
	CanvasManager::obj->resX = rX;
	CanvasManager::obj->resY = rY;

	//add the preview layer
	LayerPtr l = std::make_shared<Layer>(rX, rY);
	l->name = "Preview";
	l->opacity = 0.5f;
	CanvasManager::obj->AddLayer(l);

		if (!j.contains("layers") || !j["layers"].is_array()) {
			DLOG("no layers");
			return false;
		}

	for (const auto& layerJson : j["layers"]) {
		int id = layerJson.value("id", 0);
		std::string layer_name = layerJson.value("name", "");
		bool visible = layerJson.value("visible", true);
		float opacity = layerJson.value("opacity", 1.0f);
		int blend = layerJson.value("blend", 0);

		

		LayerPtr lp = std::make_shared<Layer>();
		lp->id = id;
		lp->name = layer_name;
		lp->visible = visible;
		lp->opacity = opacity;
		lp->blend = (BLEND_Type) blend;
		lp->image = std::make_shared<Image>(dataDir.string() + std::to_string(id) + ".png");
		CanvasManager::obj->AddLayer(lp);
	}

	CanvasManager::obj->SetSelectedLayer(-1);//max
	
return false;
}

bool ProjectManager::New()
{
	int rX, rY;
	rX = CanvasManager::obj->resX;
	rY = CanvasManager::obj->resY;
	Clear();
	//...
	LayerPtr l = std::make_shared<Layer>(rX, rY);
	l->name = "Preview";
	l->opacity = 0.5f;
	CanvasManager::obj->AddLayer(l);

	l = std::make_shared<Layer>(rX, rY);
	CanvasManager::obj->AddLayer(l);

	CanvasManager::obj->SetSelectedLayer(-1);//max
	return false;
}
bool ProjectManager::Clear()
{
	CanvasManager::Clear();

	return false;
}
