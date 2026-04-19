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
#include "input/InputFunctions.h"
#include <nlohmann/json.hpp>

bool SettingsManager::showWindow=false;
bool SettingsManager::openLatest = true;

void SettingsManager::LoadConfig()
{
    namespace fs = std::filesystem;
    //WARN: windows hardcode moment
    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile == nullptr) {
        DLOG("user not found");
        return;
    }

    fs::path configDir = fs::path(userProfile) / "AppData" / "Local" / "SagePaint";
    fs::path configFile = configDir / "current.conf";

    // Check if the file actually exists before trying to read it
    if (!fs::exists(configFile)) {
        DLOG("Config file not found, using defaults.");
        return;
    }

    std::ifstream in(configFile);
    if (in.is_open()) {
        try {
            nlohmann::json j;
            in >> j; // Parse the file content into the json object

            // Safely check and load each property
            if (j.contains("autoOpenLastFile")) {
                // Since it was saved as a string ("true" or "false")
                openLatest = (j["autoOpenLastFile"].get<std::string>() == "true");
            }

            if (j.contains("defaultResolutionX")) {
                CanvasManager::initialWidth = j["defaultResolutionX"].get<int>();
                CanvasManager::obj->resX = CanvasManager::initialWidth;
            }

            if (j.contains("defaultResolutionY")) {
                CanvasManager::initialHeight = j["defaultResolutionY"].get<int>();
                CanvasManager::obj->resY = CanvasManager::initialHeight;
            }

            if (j.contains("backgroundColor_R")) CanvasManager::backgroundColor[0] = j["backgroundColor_R"].get<float>();
            if (j.contains("backgroundColor_G")) CanvasManager::backgroundColor[1] = j["backgroundColor_G"].get<float>();
            if (j.contains("backgroundColor_B")) CanvasManager::backgroundColor[2] = j["backgroundColor_B"].get<float>();
            if (j.contains("backgroundColor_A")) CanvasManager::backgroundColor[3] = j["backgroundColor_A"].get<float>();

            if (j.contains("canvasFiltering")) {
                // std::string filtering = j["canvasFiltering"].get<std::string>();
                // Apply filtering logic here...
            }

            DLOG("Config loaded successfully.");
        }
        catch (const nlohmann::json::parse_error& e) {
            DLOG("JSON Parse Error: " << e.what());
        }
        catch (const nlohmann::json::type_error& e) {
            DLOG("JSON Type Error (corrupted config types): " << e.what());
        }

        in.close();
    }
    else {
        DLOG("Error reading file");
    }
}

void SettingsManager::LoadToolConfig()
{
    namespace fs = std::filesystem;
    //WARN: windows hardcode moment
    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile == nullptr) {
        DLOG("user not found");
        return;
    }

    fs::path configDir = fs::path(userProfile) / "AppData" / "Local" / "SagePaint";
    fs::path configFile = configDir / "tool.conf";

    if (!fs::exists(configFile)) {
        DLOG("Tool config file not found, using default tool settings.");
        return;
    }

    std::ifstream in(configFile);
    if (in.is_open()) {
        try {
            nlohmann::json j;
            in >> j; 
            if (j.contains("erase")) {
                CanvasManager::erase = j["erase"].get<bool>();
            }
            if (j.contains("presetPencil")) {
                PencilTool::LoadConfig(j["presetPencil"]);

               
            }

            if (j.contains("presetLine")) {
                LineTool::LoadConfig(j["presetLine"]);
            }

            if (j.contains("presetFill")) {
                FillTool::LoadConfig(j["presetFill"]);
            }

            if (j.contains("presetShape")) {
                ShapeTool::LoadConfig(j["presetShape"]);
            }

            if (j.contains("presetSelect")) {
                SelectTool::LoadConfig(j["presetSelect"]);
            }

            DLOG("Tool config loaded successfully.");
        }
        catch (const nlohmann::json::parse_error& e) {
            DLOG("JSON Parse Error (tool.conf): " << e.what());
        }
        catch (const nlohmann::json::type_error& e) {
            DLOG("JSON Type Error (tool.conf): " << e.what());
        }

        in.close();
    }
    else {
        DLOG("Error reading tool config file.");
    }
}

void SettingsManager::LoadBindingConfig()
{
    namespace fs = std::filesystem;
    //WARN: windows hardcode moment
    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile == nullptr) {
        DLOG("user not found");
        return;
    }

    fs::path configDir = fs::path(userProfile) / "AppData" / "Local" / "SagePaint";
    fs::path configFile = configDir / "binding.conf";

    if (!fs::exists(configFile)) {
        DLOG("Binding config file not found, using default bindings.");
        InputManager::keyMap.Default(0);
        return;
    }

    std::ifstream in(configFile);
    if (in.is_open()) {
        try {
            nlohmann::json j;
            in >> j;

            if (j.contains("keyMaps") && j["keyMaps"].is_array()) {
                // Loop through every saved tool/binding object in the JSON
                for (const auto& savedMap : j["keyMaps"]) {
                    if (!savedMap.contains("name")) continue;

                    std::string funcName = savedMap["name"].get<std::string>();

                    // Match the saved name to the actual FunctionData in your array
                    for (int i = 0; i < inputFunctions.size(); i++) {
                        FunctionData& fd = inputFunctions[i];

                        if (fd.name == funcName) {

                            // NOTE: If you pre-populate inputFunctions with default keys on startup,
                            // you may want to call your RemoveBind/Clear logic here before adding the loaded ones!

                            // Load DOWN bindings (mode = 0)
                            if (savedMap.contains("key") && savedMap["key"].is_array()) {
                                for (int keyId : savedMap["key"]) {
                                    fd.AddBind(keyId, 0);
                                }
                            }

                            // Load HOLD bindings (mode = 1)
                            if (savedMap.contains("keyHold") && savedMap["keyHold"].is_array()) {
                                for (int keyId : savedMap["keyHold"]) {
                                    fd.AddBind(keyId, 1);
                                }
                            }

                            // Load UP bindings (mode = 2)
                            if (savedMap.contains("keyUp") && savedMap["keyUp"].is_array()) {
                                for (int keyId : savedMap["keyUp"]) {
                                    fd.AddBind(keyId, 2);
                                }
                            }

                            break; // We found the matching tool, move to the next JSON object
                        }
                    }
                }
            }
            DLOG("BINDING CONFIG LOADED: " << configFile.string());
        }
        catch (const nlohmann::json::exception& e) {
            // Catches both parse_error and type_error
            DLOG("JSON Error (binding.conf): " << e.what());
        }
        in.close();
    }
    else {
        DLOG("ERROR: READING FILE " << configFile.string());
    }
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
	"defaultResolutionX": )" << CanvasManager::initialWidth << R"(,
	"defaultResolutionY": )" << CanvasManager::initialHeight << R"(,
    "backgroundColor_R": )" << CanvasManager::backgroundColor[0] << R"(,
    "backgroundColor_G": )" << CanvasManager::backgroundColor[1] << R"(,
    "backgroundColor_B": )" << CanvasManager::backgroundColor[2] << R"(,
    "backgroundColor_A": )" << CanvasManager::backgroundColor[3] << R"(,
    "canvasFiltering": )" << "\"NEAREST\"";

   

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
    "erase": )"<<(CanvasManager::erase?"true":"false")<<R"(,
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

void SettingsManager::SaveBindingConfig()
{
    namespace fs = std::filesystem;
    //WARN: windows hardcode moment
    const char* userProfile = std::getenv("USERPROFILE");

    if (userProfile == nullptr) {
        DLOG("user not found");
        return;
    }

    fs::path configDir = fs::path(userProfile) / "AppData" / "Local" / "SagePaint";
    fs::path configFile = configDir / "binding.conf";

    std::error_code ec;
    fs::create_directories(configDir, ec);
    if (ec) {
        DLOG("Failed to create directory: " << ec.message());
        return;
    }

    std::ofstream out(configFile);
    if (out.is_open()) {

        out << "{\n";
        out << "  \"keyMaps\": [\n"; 

        auto WriteIntArray = [&](const std::string& keyName, const std::vector<int>& vec, bool isLast) {
            out << "      \"" << keyName << "\": [";
            for (size_t j = 0; j < vec.size(); j++) {
                out << vec[j];
                if (j < vec.size() - 1) out << ", ";
            }
            out << "]";
            if (!isLast) out << ","; 
            out << "\n";
            };

        for (int i = 0; i < inputFunctions.size(); i++) {
            FunctionData& fd = inputFunctions[i];

            out << "    {\n";
            out << "      \"name\": \"" << fd.name << "\",\n"; 

            WriteIntArray("key", fd.key, false);
            WriteIntArray("keyHold", fd.keyHold, false);
            WriteIntArray("keyUp", fd.keyUp, true); 

            out << "    }";

            if (i < inputFunctions.size() - 1) {
                out << ",";
            }
            out << "\n";
        }

        out << "  ]\n"; 
        out << "}\n";   

        out.close();
        DLOG("BINDING CONFIG SAVED: " << configFile.string());
    }
    else {
        DLOG("ERROR: WRITING TO FILE " << configFile.string());
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
            static bool init = true;
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
            
            if (ImGui::Button("Revert changes")||init) {
                init = false;
                open_latest = openLatest;
                res_height = CanvasManager::initialHeight;
                res_width = CanvasManager::initialWidth;
                bg_clr[0] = CanvasManager::backgroundColor[0];
                bg_clr[1] = CanvasManager::backgroundColor[1];
                bg_clr[2] = CanvasManager::backgroundColor[2];
                bg_clr[3] = CanvasManager::backgroundColor[3];
                canvas_filtering = (int)CanvasManager::canvasFiltering;
            }
            if (ImGui::Button("Save tools as default")) {
                SaveToolConfig();
            }ImGui::SameLine();
            if (ImGui::Button("Load tool settings")) {
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