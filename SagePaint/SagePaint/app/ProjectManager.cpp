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
#include "SettingsManager.h"

enum ProjectAction {
	NONE,
	SAVE,
	OPEN,
	EXIT,
	NEW,
	OPENBUFFEREDPATH
};

bool ProjectManager::dirty = false;
std::string ProjectManager::name = "New Project";
std::string ProjectManager::description = "...";

bool ProjectManager::projectDataDirty = false;
std::string ProjectManager::fullPath = "C:\\temp\\new.sagepaint";


ProjectAction bufferedAction = NONE;
std::string bufferedPath = "";
bool show_exit_popup = false;
bool show_new_popup = false;
bool show_save_popup = false;
bool show_export_popup = false;
bool show_open_popup = false;
bool show_opensave_popup = false;
bool show_resize_popup = false;

bool show_project_window = false;

bool ProjectManager::ShowFileUI()
{
	

	static std::string open_filename = fullPath;
	static std::string save_filename = fullPath;
	static std::string export_filename = fullPath;

	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New", "Shortcut")) {
			if (dirty) {
				show_new_popup = true;
			}
			else {
				New();
			}
		}
		if (ImGui::MenuItem("Open", "Shortcut")) {
			if (dirty) {
				show_opensave_popup = true;
				bufferedAction = OPEN;
			}
			else {
				show_open_popup = true;
			}
		}
		if (ImGui::BeginMenu("Open Recent"))
		{
			for (size_t i = 0; i < SettingsManager::recentFiles.size(); ++i)
			{
				const std::string& file = SettingsManager::recentFiles[i];

				if (ImGui::MenuItem(file.c_str()))
				{
					if (dirty)
					{
						show_opensave_popup = true;
						bufferedAction = OPENBUFFEREDPATH;
						bufferedPath = file; 
					}
					else
					{
						Open(file);
					}
				}
			}

			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Save", "Shortcut")) {
			if (dirty) {
				show_save_popup = true;
			}
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
				return false;//exits
			}
			//runApp = false;
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Export")) {
			show_export_popup = true;
			}

		if (ImGui::MenuItem("Resize")) {
			show_resize_popup = true;
		}
		ImGui::Separator();
		if (ImGui::MenuItem("Project info")) {
			show_project_window = true;
		}


		ImGui::EndMenu();
	}
		if (show_new_popup)
		{
			ImGui::OpenPopup("Save before new?");
			show_new_popup = false;

			save_filename = fullPath;
		}
		if (show_exit_popup)
		{
			ImGui::OpenPopup("Save Changes?");
			show_exit_popup = false; 

			save_filename = fullPath;
		}
		if (show_save_popup) {
			ImGui::OpenPopup("Save As?");
			show_save_popup = false;

			save_filename = fullPath;
		}
		if (show_open_popup) {
			ImGui::OpenPopup("Open project");
			show_open_popup = false;
			open_filename = fullPath;
		}if (show_opensave_popup) {
			ImGui::OpenPopup("Save before Opening?");
			show_opensave_popup = false;
			open_filename = fullPath;
		}
		if (show_export_popup) {
			ImGui::OpenPopup("Export image to:");
			show_export_popup = false;
			export_filename = fullPath;
			
		}
		static std::string project_name;
		static std::string project_description;
		if (show_project_window) {
			ImGui::OpenPopup("Project information");
			show_project_window = false;
			project_name = name;
			project_description = description;

		}
		if (ImGui::BeginPopupModal("Project information", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			
			ImGui::InputText("##project_name", &project_name);
			
			ImGui::InputText("##project_desc", &project_description);




			if (ImGui::Button("Save changes", ImVec2(138, 0)))
			{
				name = project_name;
				description = project_description;
				projectDataDirty = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		static int rX = 640;
		static int oX = 640;
		static int rY = 0;
		static int oY = 0;

		if (show_resize_popup) {
			ImGui::OpenPopup("Resize");
			show_resize_popup = false;
			rX = CanvasManager::obj->resX;
			rY = CanvasManager::obj->resY;
			oX = 0;
			oY = 0;
		}
		if (ImGui::BeginPopupModal("Resize", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputInt("##resizeX", &rX); ImGui::SameLine();
			ImGui::InputInt("##resizeY", &rY);

			ImGui::InputInt("##offsetX", &oX); ImGui::SameLine();
			ImGui::InputInt("##offsetY", &oY);


			if (ImGui::Button("Resize", ImVec2(138, 0)))
			{
				CanvasManager::Crop(rX,rY,oX,oY);
				CanvasManager::Init();//reset pos
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		//export
		if (ImGui::BeginPopupModal("Export image to:", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter file name or path to export:");

			ImGui::InputText("##export_filename", &export_filename);

			ImGui::Separator();

			if (ImGui::Button("Export", ImVec2(120, 0)))
			{
				CanvasManager::Export(export_filename);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		//OPEN
		if (ImGui::BeginPopupModal("Open project", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter file name or path to open:");


			ImGui::InputText("##open_filename", &open_filename);

			ImGui::Separator();

			if (ImGui::Button("Open", ImVec2(120, 0)))
			{
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
		


		if (ImGui::BeginPopupModal("Save As?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter file name or path:");

		
			ImGui::InputText("##save_filename", &save_filename);

			ImGui::Separator();

			if (ImGui::Button("Save", ImVec2(120, 0)))
			{

				Save(save_filename);
				SettingsManager::SaveProjectPathToRecent();
				if (bufferedAction == OPEN) {
					bufferedAction = NONE;
					show_open_popup = true;
					open_filename = fullPath;
				}
				else if (bufferedAction == EXIT){//TODO:let save be save OR save as
					bufferedAction = NONE;

					save_filename = fullPath;

					ImGui::CloseCurrentPopup();

					ImGui::EndPopup();
					return false;
				}
				else if (bufferedAction == NEW) {
					bufferedAction = NONE;
					save_filename = fullPath;
					New();

				}
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
			if (ImGui::BeginPopupModal("Save before Opening?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Save before opening?");
				ImGui::Separator();


				if (ImGui::Button("Save", ImVec2(120, 0))) {

					show_save_popup = true;
					
					//bufferedAction = OPEN;
					ImGui::CloseCurrentPopup();

				}
				ImGui::SameLine();
				if (ImGui::Button("Save As", ImVec2(120, 0))) {

					show_save_popup = true;
					//bufferedAction = OPEN;
					ImGui::CloseCurrentPopup();

				}
				if (ImGui::Button("Skip", ImVec2(120, 0))) {
					if (bufferedAction == OPENBUFFEREDPATH) {
						Open(bufferedPath);
					}
					else {
						show_open_popup = true;

					}
					ImGui::CloseCurrentPopup();
					
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();


				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					bufferedAction = NONE;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

				if (ImGui::BeginPopupModal(	"Save before new?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("Save before starting new project?");
					ImGui::Separator();


					if (ImGui::Button("Save", ImVec2(120, 0))) {

						show_save_popup = true;

						bufferedAction = NEW;
						ImGui::CloseCurrentPopup();

					}
					ImGui::SameLine();
					if (ImGui::Button("Save As", ImVec2(120, 0))) {

						show_save_popup = true;
						bufferedAction = NEW;
						ImGui::CloseCurrentPopup();

					}
					if (ImGui::Button("Skip", ImVec2(120, 0))) {

						New();
						ImGui::CloseCurrentPopup();

					}
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();


					if (ImGui::Button("Cancel", ImVec2(120, 0))) {
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
				bufferedAction = EXIT;
				ImGui::CloseCurrentPopup();
				
			}
			ImGui::SameLine();
			if (ImGui::Button("Save As", ImVec2(120, 0))) {

				bufferedAction = EXIT;
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

bool ProjectManager::ExitDlgOpen()
{
	if (dirty) {
		show_exit_popup = true;
	}
	else {
		return false;//exits
	}
	return true;
}

bool ProjectManager::Save(std::string path) //path may be folder, or a file to override. only override .sagepaint files, otherwise
{
	
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
    "version": "0.68",
    "description": ")" << description << R"(",
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

	name = targetPath.filename().string();
	fullPath = targetPath.string();
	//Open(path);//Test
	dirty = false;
	projectDataDirty = true;
	return false;
}



bool ProjectManager::Open(std::string path)
{



	namespace fs = std::filesystem;
	fs::path targetPath(path);

	if (!fs::exists(targetPath)) {
		DLOG("file doesnt exist")
			return false;
	}
	unsigned int rX, rY;
	if (fs::is_regular_file(targetPath)) {
		std::string ext = targetPath.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
			Clear();

			name = targetPath.filename().string();
			description = "...";
			//std::string version;
			//std::string description = j.value("description", "");
			
			LayerPtr image = std::make_shared<Layer>(targetPath.string());
			image->name = targetPath.filename().string();

			rX = image->image->width;
			rY = image->image->height;
			//validate
			//CanvasManager::obj->resX = rX;
			//CanvasManager::obj->resY = rY;

			LayerPtr l = std::make_shared<Layer>(rX, rY);
			l->name = "Preview";
			l->opacity = 0.5f;
			CanvasManager::obj->AddLayer(l);
			CanvasManager::obj->AddLayer(image);

		}
	}
	else if (fs::is_directory(targetPath)) {
		fs::path dataDir = targetPath / "data" / "";
		fs::path configFile = targetPath / "project.conf";


		using json = nlohmann::json;

		std::ifstream in(configFile);
		if (!in) {
			DLOG("failed to read config file");
			return 1;
		}

		Clear();//clear data
		json j;
		in >> j;

		name = j.value("name", "");
		std::string version = j.value("version", "");
		description = j.value("description", "");
		
		rX = j.value("resolutionX", 640);
		rY = j.value("resolutionY", 400);
		//validate
		//CanvasManager::obj->resX = rX;
		//CanvasManager::obj->resY = rY;
		
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
			lp->blend = (BLEND_Type)blend;
			lp->image = std::make_shared<Image>(dataDir.string() + std::to_string(id) + ".png");
			CanvasManager::obj->AddLayer(lp);
		}
	}
	

	fullPath = targetPath.string();
	CanvasManager::Init();//set transform to default
	CanvasManager::obj->SetSelectedLayer(-1);//max
	CanvasManager::ResChange(rX,rY);
	CanvasManager::obj->SetZoomCached();
	CanvasManager::obj->Changed();
	
	projectDataDirty = true;
	dirty = false;
	SettingsManager::SaveProjectPathToRecent();
	
return true;
}

bool ProjectManager::New()
{
	int rX, rY;
	rX = CanvasManager::obj->resX;
	rY = CanvasManager::obj->resY;
	Clear();
	CanvasManager::ResChange(rX,rY);
	//...
	LayerPtr l = std::make_shared<Layer>(rX, rY);
	l->name = "Preview";
	l->opacity = 0.5f;
	CanvasManager::obj->AddLayer(l);

	l = std::make_shared<Layer>(rX, rY);
	CanvasManager::obj->AddLayer(l);

	name = "New project";
	CanvasManager::obj->SetSelectedLayer(-1);//max
	CanvasManager::Init();//set transform to default
	CanvasManager::obj->Changed();
	projectDataDirty = true;
	dirty = false;

	return false;
}

bool ProjectManager::Clear()
{
	CanvasManager::Clear();

	return false;
}

void ProjectManager::Dirty()
{
	dirty = true;
}
