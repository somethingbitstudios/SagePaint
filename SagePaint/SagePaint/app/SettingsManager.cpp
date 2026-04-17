#include "SettingsManager.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <debug.h>
#include "CanvasManager.h"
#include "tools/PencilTool.h"
#include "tools/LineTool.h"
#include "tools/FillTool.h"
#include "tools/ShapeTool.h"
#include "tools/SelectTool.h"
#include <imgui.h>


bool SettingsManager::showWindow=false;
bool SettingsManager::openLatest = true;
void SettingsManager::LoadConfig()
{

}

void SettingsManager::LoadToolConfig()
{

}

void SettingsManager::SaveConfig()
{
    namespace fs = std::filesystem;
    //WARN: windows hardcode moment
    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile == nullptr) {
        DLOG("user not found")
        return;
    }

    fs::path configDir = fs::path(userProfile) / "AppData" / "Local" / "SagePaint";
    fs::path configFile = configDir / "current.conf";

    std::error_code ec;
    fs::create_directories(configDir, ec);
    if (ec) {
        DLOG( "Failed to create directory: " << ec.message());
        return;
    }

    std::ofstream out(configFile);
    if (out.is_open()) {


        out << R"({
    "version": "0.68",
    "autoOpenLastFile": ")" << ((openLatest) ? "true" : "false") << R"(",
	"defaultResolutionX": )" << CanvasManager::obj->resX << R"(,
	"defaultResolutionY": )" << CanvasManager::obj->resY << R"(,
    "backgroundColor_R": )" << CanvasManager::backgroundColor[0] << R"(,
    "backgroundColor_G": )" << CanvasManager::backgroundColor[1] << R"(,
    "backgroundColor_B": )" << CanvasManager::backgroundColor[2] << R"(,
    "backgroundColor_A": )" << CanvasManager::backgroundColor[3] << R"(,
    "canvasFiltering": )" << "NEAREST";

   

        out << "\n}";
        out.close();
    }
    else {
       DLOG("Error writing to file")
    }
}
void SettingsManager::SaveToolConfig()
{
    namespace fs = std::filesystem;
    //WARN: windows hardcode moment
    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile == nullptr) {
        DLOG("user not found")
            return;
    }

    fs::path configDir = fs::path(userProfile) / "AppData" / "Local" / "SagePaint";
    fs::path configFile = configDir / "tool.conf";

    std::error_code ec;
    fs::create_directories(configDir, ec);
    if (ec) {
        DLOG("Failed to create directory: " << ec.message());
        return;
    }

    std::ofstream out(configFile);
    if (out.is_open()) {


        out << R"({
    "presetPencil":
)" << PencilTool::ConfigString() << R"(,
    "presetLine":
)" << LineTool::ConfigString() << R"(,
    "presetFill":
)" << FillTool::ConfigString() << R"(,
    "presetShape":
)" << ShapeTool::ConfigString() << R"(,
    "presetSelect":
)" << SelectTool::ConfigString();








        out << "\n}";
        out.close();
    }
    else {
        DLOG("Error writing to file")
    }
}
void SettingsManager::ShowUI() {
    if (showWindow)
    {
        if (ImGui::Begin("Settings", &showWindow))
        {
            static bool open_latest = false;
            static int res_width = 640;
            static int res_height = 400;
            static float bg_clr[4] = {0,0,0,1};
            static int canvas_filtering = 0;
            if (ImGui::Button("Save changes")) {
                openLatest = open_latest;
                CanvasManager::initialHeight = res_height;
                CanvasManager::initialWidth = res_width;
                CanvasManager::backgroundColor[0] = bg_clr[0];
                CanvasManager::backgroundColor[1] = bg_clr[1];
                CanvasManager::backgroundColor[2] = bg_clr[2];
                CanvasManager::backgroundColor[3] = bg_clr[3];
                CanvasManager::canvasFiltering = (CanvasFilteringMode)canvas_filtering;
                SettingsManager::SaveConfig();
            }
            ImGui::SameLine();
            if (ImGui::Button("Revert changes")) {
                open_latest = openLatest;
                res_height = CanvasManager::initialHeight;
                res_width = CanvasManager::initialWidth;
                bg_clr[0] = CanvasManager::backgroundColor[0];
                bg_clr[1] = CanvasManager::backgroundColor[1];
                bg_clr[2] = CanvasManager::backgroundColor[2];
                bg_clr[3] = CanvasManager::backgroundColor[3];
                canvas_filtering = (int)CanvasManager::canvasFiltering;
            }
            if (ImGui::Button("Save tool settings as default")) {
                SaveToolConfig();
            }ImGui::SameLine();
            if (ImGui::Button("Load default tool settings")) {
                LoadToolConfig();
            }

            ImGui::Checkbox("Open latest file on startup",&open_latest);
            ImGui::Text("Default canvas resolution");
            if (ImGui::InputInt("width", &res_width,1,40)) {
                res_width = std::max(1, res_width);
            }
            if (ImGui::InputInt("height", &res_height,1,40)) {
                res_height = std::max(1, res_height);
            }

            ImGui::ColorEdit4("Background", (float*)&bg_clr);

            const char* blend_modes[] = { "Nearest", "Bilinear", "Complex" };
            ImGui::Combo("Blend", &(canvas_filtering), blend_modes, IM_ARRAYSIZE(blend_modes));

            ImGui::Separator();

            //input
            InputManager::ShowUI();
        }
        ImGui::End();
    }
}