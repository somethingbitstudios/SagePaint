#include "InputManager.h"
#include "InputFunctions.h"
#include <array>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "../SettingsManager.h"
#include <chrono>
//#include "../CanvasManager.h"

int InputManager::modsPersistent = -1;
InputManager::InputManager() {

}
InputManager::~InputManager() {

}
void InputManager::ShowUI() {
    ImGui::Text("Input binding:");
    ImGui::SameLine();
    if (ImGui::Button("Save")) {
        SettingsManager::SaveBindingConfig();
    }
    ImGui::SameLine();
    if (ImGui::Button("Revert changes ")) {
        Init();
        SettingsManager::LoadBindingConfig();
    }
    ImGui::SameLine();
    if (ImGui::Button("Default")) {
        Init();
        keyMap.Default(0);
    }

    static int listening_row = -1;   
    static int listening_action = -1; 
    static bool wait_for_release = false;

    static std::vector<std::array<int, 3>> pending_keys;

    if (pending_keys.size() != inputFunctions.size()) {
        pending_keys.resize(inputFunctions.size(), { GLFW_KEY_UNKNOWN, GLFW_KEY_UNKNOWN, GLFW_KEY_UNKNOWN });
    }
  
    for (int k = 0; k < inputFunctions.size(); k++) {
        ImGui::SetWindowFontScale(1.5f); 
        ImGui::Text(inputFunctions[k].name.c_str());
        ImGui::SetWindowFontScale(1.f);
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 400.0f);
        ImGui::TextColored(ImVec4(0.5, 0, 0.5, 1), inputFunctions[k].description.c_str());
        ImGui::Text("bound to:");

        ImGui::PushID(k); 
        //lambda gaming
        auto DrawBindSection = [&](int action_type, const char* label, std::vector<int>& current_binds) {
            ImGui::PushID(action_type); 

            ImGui::Text("%s", label);
            ImGui::SameLine();

            int& bound_glfw_key = pending_keys[k][action_type];
         
            bool is_listening = (listening_row == k && listening_action == action_type);
            
            std::string button_text = "None";

            static bool consumeup = false;
            if (bound_glfw_key != GLFW_KEY_UNKNOWN) {
                std::string key_name = GetGLFWKeyName(bound_glfw_key);
                if (key_name != "") {
                    button_text = key_name;
                }
                else {
                    button_text = "ID:" + std::to_string(bound_glfw_key);
                }
               
                


            }else if (is_listening&&!wait_for_release) {
                button_text = "Press any key...";
            }
            if (ImGui::Button(button_text.c_str(), ImVec2(120, 0))&&!wait_for_release) {
                if (consumeup&&bound_glfw_key==GLFW_MOUSE_BUTTON_LEFT) { consumeup = false; }
                else {
                    listening_row = k;
                    listening_action = action_type;
                    bound_glfw_key = GLFW_KEY_UNKNOWN;
                    wait_for_release = true;
                }
            }

            if (is_listening) {
                GLFWwindow* my_window = glfwGetCurrentContext();

                if (wait_for_release) {
                    bool is_anything_down = false;

                    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
                        if (glfwGetMouseButton(my_window, i) == GLFW_PRESS) is_anything_down = true;
                    }
                    DLOG("dunk")
                    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_KP_EQUAL; i++) {//excludes
                        if (glfwGetKey(my_window, i) == GLFW_PRESS) {
                            is_anything_down = true;
                            DLOG("func")

                        }
                        }

                    if (!is_anything_down) {
                        wait_for_release = false;
                    }
                }
                else {
                    bool input_detected = false;

                    for (int key_code = GLFW_KEY_SPACE; key_code <= GLFW_KEY_KP_EQUAL; key_code++) {
                        if (glfwGetKey(my_window, key_code) == GLFW_PRESS) {
                            if (key_code == GLFW_KEY_ESCAPE) {
                                listening_row = -1; listening_action = -1;
                            }
                            else {
                                bound_glfw_key = key_code+modsPersistent*1000;

                                listening_row = -1; listening_action = -1;
                            }
                            input_detected = true;
                            break;
                        }
                    }

                    if (!input_detected) {
                        for (int mouse_btn = GLFW_MOUSE_BUTTON_1; mouse_btn <= GLFW_MOUSE_BUTTON_LAST; mouse_btn++) {
                            if (glfwGetMouseButton(my_window, mouse_btn) == GLFW_PRESS) {
                                bound_glfw_key = mouse_btn + modsPersistent * 1000;
                                consumeup = true;
                                listening_row = -1; listening_action = -1;
                                break;
                            }
                        }
                    }
                }
            }

            ImGui::SameLine();

            if (ImGui::Button("Add bind")) {
                if (bound_glfw_key != GLFW_KEY_UNKNOWN) {
                   
                    inputFunctions[k].AddBind(bound_glfw_key, action_type);//handle key being > 1000
                    bound_glfw_key = GLFW_KEY_UNKNOWN; 
                }
            }

            for (int i = 0; i < current_binds.size(); i++) {
             
                ImGui::PushID(i + 10000); 

                if (ImGui::Button("Remove bind")) {
                    inputFunctions[k].RemoveBind(current_binds[i], action_type);
                    ImGui::PopID();
                    break;
                }

                ImGui::PopID();

                ImGui::SameLine();
                std::string bound_name = GetGLFWKeyName(current_binds[i]);
                ImGui::TextColored(ImVec4(0.8, 0, 0.8, 1), "%s", !bound_name.empty() ? bound_name.c_str() : "Unknown");
            
            }

            ImGui::PopID(); 
            };

        DrawBindSection(0, "   On Down:", inputFunctions[k].key);
        DrawBindSection(1, "   On hold:", inputFunctions[k].keyHold);
        DrawBindSection(2, "   On Up:  ", inputFunctions[k].keyUp);

        ImGui::PopID(); 
    }
}
void InputManager::Init() {

	SetContext(KEY_CONTEXT_DEFAULT);//WARN: set to ui instead of default for testing only
	InitInputFunctions();

    keyMap.Init();
}
void InputManager::LoadInputMap(std::string path) {

}
void InputManager::Input(int key, int action, int mods) {
    key += mods * 1000;
	//key.held = true;
	if (action==1) {
		keyHeld.emplace_back(key);
        modsPersistent = mods;
        //DLOG("mods: "<<mods)
	}
	else if (action==0){//key.held = false;
        modsPersistent = 0;
        auto it = std::find_if(keyHeld.begin(), keyHeld.end(),
            [&](const auto& p) { return p%1000 == key%1000;/*any mods*/ });

        if (it != keyHeld.end()) {
            keyHeld.erase(it);
        }
	}
	keyMap.Action(key, action);
}
void InputManager::ProcessHeld() {
	for (int i = 0; i < keyHeld.size(); i++) {
		keyMap.Action(keyHeld[i], 3);//WARN: held inputs have NO mods for now, which means you need to manually check if ctrl etc.
	}
}
void InputManager::SetContext(Key_Context_Enum keyContext) {
	InputMap::SetContext(keyContext);
}
void InputManager::UpdateCursorPos(GLFWwindow* window) {
	cursorXold = cursorX.load();
	cursorYold = cursorY.load();
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	cursorX = x;
	cursorY = y;

} 
void InputManager::SetCursorPos(float x, float y) {
	cursorXold = cursorX.load();
	cursorYold = cursorY.load();
	
	cursorX = x; 
	cursorY = y;


}

std::atomic<float> InputManager::cursorX;
std::atomic<float> InputManager::cursorY;
std::atomic<float> InputManager::cursorYold;
std::atomic<float> InputManager::cursorXold;
InputMap InputManager::keyMap;
std::vector<int> InputManager::keyHeld;


std::string InputManager::GetGLFWKeyName(int key) {
    int mod = key / 1000;
    key -= mod * 1000;
    std::string keyString="?";
    std::string modString;


    switch (mod) {
    case 0: modString = ""; break;
    case 1: modString = "Shift + "; break;
    case 2: modString = "Ctrl + "; break;
    case 3: modString = "Ctrl + Shift+ "; break;
    case 4: modString = "Alt + "; break;
    case 5: modString = "Alt + Shift + "; break;
    case 6: modString = "Ctrl + Alt + "; break;
    case 7: modString = "Ctrl + Alt + Shift + "; break;
    }
    
    
        switch (key) {
        case GLFW_MOUSE_BUTTON_LEFT: keyString = "Mouse left"; break;
        case GLFW_MOUSE_BUTTON_MIDDLE: keyString = "Mouse middle"; break;
        case GLFW_MOUSE_BUTTON_RIGHT: keyString = "Mouse right"; break;
        case GLFW_KEY_SPACE: keyString = "Space"; break;
        case GLFW_KEY_ESCAPE: keyString = "Escape"; break;
        case GLFW_KEY_ENTER: keyString = "Enter"; break;
        case GLFW_KEY_TAB: keyString = "Tab"; break;
        case GLFW_KEY_BACKSPACE: keyString = "Backspace"; break;
        case GLFW_KEY_INSERT: keyString = "Insert"; break;
        case GLFW_KEY_DELETE: keyString = "Delete"; break;
        case GLFW_KEY_RIGHT: keyString = "Right Arrow"; break;
        case GLFW_KEY_LEFT: keyString = "Left Arrow"; break;
        case GLFW_KEY_DOWN: keyString = "Down Arrow"; break;
        case GLFW_KEY_UP: keyString = "Up Arrow"; break;
        case GLFW_KEY_PAGE_UP: keyString = "Page Up"; break;
        case GLFW_KEY_PAGE_DOWN: keyString = "Page Down"; break;
        case GLFW_KEY_HOME: keyString = "Home"; break;
        case GLFW_KEY_END: keyString = "End"; break;
        case GLFW_KEY_CAPS_LOCK: keyString = "Caps Lock"; break;
        case GLFW_KEY_SCROLL_LOCK: keyString = "Scroll Lock"; break;
        case GLFW_KEY_NUM_LOCK: keyString = "Num Lock"; break;
        case GLFW_KEY_PRINT_SCREEN: keyString = "Print Screen"; break;
        case GLFW_KEY_PAUSE: keyString = "Pause"; break;
        case GLFW_KEY_LEFT_SHIFT: keyString = "Left Shift"; break;
        case GLFW_KEY_LEFT_CONTROL: keyString = "Left Ctrl"; break;
        case GLFW_KEY_LEFT_ALT: keyString = "Left Alt"; break;
        case GLFW_KEY_LEFT_SUPER: keyString = "Left Super"; break;
        case GLFW_KEY_RIGHT_SHIFT: keyString = "Right Shift"; break;
        case GLFW_KEY_RIGHT_CONTROL: keyString = "Right Ctrl"; break;
        case GLFW_KEY_RIGHT_ALT: keyString = "Right Alt"; break;
        case GLFW_KEY_RIGHT_SUPER: keyString = "Right Super"; break;
        case GLFW_KEY_MENU: keyString = "Menu"; break;

        case GLFW_KEY_F1: keyString = "F1"; break;
        case GLFW_KEY_F2: keyString = "F2"; break;
        case GLFW_KEY_F3: keyString = "F3"; break;
        case GLFW_KEY_F4: keyString = "F4"; break;
        case GLFW_KEY_F5: keyString = "F5"; break;
        case GLFW_KEY_F6: keyString = "F6"; break;
        case GLFW_KEY_F7: keyString = "F7"; break;
        case GLFW_KEY_F8: keyString = "F8"; break;
        case GLFW_KEY_F9: keyString = "F9"; break;
        case GLFW_KEY_F10: keyString = "F10"; break;
        case GLFW_KEY_F11: keyString = "F11"; break;
        case GLFW_KEY_F12: keyString = "F12"; break;

        case GLFW_KEY_KP_0: keyString = "Numpad 0"; break;
        case GLFW_KEY_KP_1: keyString = "Numpad 1"; break;
        case GLFW_KEY_KP_2: keyString = "Numpad 2"; break;
        case GLFW_KEY_KP_3: keyString = "Numpad 3"; break;
        case GLFW_KEY_KP_4: keyString = "Numpad 4"; break;
        case GLFW_KEY_KP_5: keyString = "Numpad 5"; break;
        case GLFW_KEY_KP_6: keyString = "Numpad 6"; break;
        case GLFW_KEY_KP_7: keyString = "Numpad 7"; break;
        case GLFW_KEY_KP_8: keyString = "Numpad 8"; break;
        case GLFW_KEY_KP_9: keyString = "Numpad 9"; break;

        case GLFW_KEY_KP_DECIMAL: keyString = "Numpad ."; break;
        case GLFW_KEY_KP_DIVIDE: keyString = "Numpad /"; break;
        case GLFW_KEY_KP_MULTIPLY: keyString = "Numpad *"; break;
        case GLFW_KEY_KP_SUBTRACT: keyString = "Numpad -"; break;
        case GLFW_KEY_KP_ADD: keyString = "Numpad +"; break;
        case GLFW_KEY_KP_ENTER: keyString = "Numpad Enter"; break;
        case GLFW_KEY_KP_EQUAL: keyString = "Numpad ="; break;

        case GLFW_KEY_SEMICOLON: keyString = ";"; break;
        case GLFW_KEY_1: keyString = "1"; break;
        case GLFW_KEY_2: keyString = "2"; break;
        case GLFW_KEY_3: keyString = "3"; break;
        case GLFW_KEY_4: keyString = "4"; break;
        case GLFW_KEY_5: keyString = "5"; break;
        case GLFW_KEY_6: keyString = "6"; break;
        case GLFW_KEY_7: keyString = "7"; break;
        case GLFW_KEY_8: keyString = "8"; break;
        case GLFW_KEY_9: keyString = "9"; break;
        case GLFW_KEY_0: keyString = "0"; break;
        }
    
        if (keyString == "?") {
            const char* name = glfwGetKeyName(key, 0);

            if (name) { keyString = name; }
        }
    
    return (modString+keyString);
}