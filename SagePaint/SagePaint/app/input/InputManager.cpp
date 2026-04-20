#include "InputManager.h"
#include "InputFunctions.h"
#include <array>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "../SettingsManager.h"
//#include "../CanvasManager.h"
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
            if (is_listening&&!wait_for_release) {
                button_text = "Press any key...";
            }
            else if (bound_glfw_key != GLFW_KEY_UNKNOWN) {
                const char* key_name = GetGLFWKeyName(bound_glfw_key);
                if (key_name) {
                    button_text = key_name;
                }
                else {
                    button_text = "Key ID: " + std::to_string(bound_glfw_key);
                }
            }

            if (ImGui::Button(button_text.c_str(), ImVec2(120, 0))&&!wait_for_release) {
                
                listening_row = k;
                listening_action = action_type;
                bound_glfw_key = GLFW_KEY_UNKNOWN;
                wait_for_release = true; 
            }

            if (is_listening) {
                GLFWwindow* my_window = glfwGetCurrentContext();

                if (wait_for_release) {
                    bool is_anything_down = false;

                    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
                        if (glfwGetMouseButton(my_window, i) == GLFW_PRESS) is_anything_down = true;
                    }
                    for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; i++) {
                        if (glfwGetKey(my_window, i) == GLFW_PRESS) is_anything_down = true;
                    }

                    if (!is_anything_down) {
                        wait_for_release = false;
                    }
                }
                else {
                    bool input_detected = false;

                    for (int key_code = GLFW_KEY_SPACE; key_code <= GLFW_KEY_LAST; key_code++) {
                        if (glfwGetKey(my_window, key_code) == GLFW_PRESS) {
                            if (key_code == GLFW_KEY_ESCAPE) {
                                listening_row = -1; listening_action = -1;
                            }
                            else {
                                bound_glfw_key = key_code;
                                listening_row = -1; listening_action = -1;
                            }
                            input_detected = true;
                            break;
                        }
                    }

                    if (!input_detected) {
                        for (int mouse_btn = GLFW_MOUSE_BUTTON_1; mouse_btn <= GLFW_MOUSE_BUTTON_LAST; mouse_btn++) {
                            if (glfwGetMouseButton(my_window, mouse_btn) == GLFW_PRESS) {
                                bound_glfw_key = mouse_btn;
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
                    inputFunctions[k].AddBind(bound_glfw_key, action_type);
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
                const char* bound_name = GetGLFWKeyName(current_binds[i]);
                ImGui::TextColored(ImVec4(0.8, 0, 0.8, 1), "%s", bound_name ? bound_name : "Unknown");
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
	//key.held = true;
	if (action==1) {
		keyHeld.emplace_back(key);
	}
	else if (action==0){//key.held = false;
		auto it = std::find(keyHeld.begin(), keyHeld.end(), key);
		if (it != keyHeld.end()) {
			keyHeld.erase(it);
		}
	}
	keyMap.Action(key, action, mods);
}
void InputManager::ProcessHeld() {
	for (int i = 0; i < keyHeld.size(); i++) {
		keyMap.Action(keyHeld[i], 3, 0);//WARN: held inputs have NO mods for now, which means you need to manually check if ctrl etc.
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


const char* InputManager::GetGLFWKeyName(int key) {
   

    switch (key) {
    case GLFW_MOUSE_BUTTON_LEFT: return "Mouse left";
    case GLFW_MOUSE_BUTTON_MIDDLE: return "Mouse middle";
    case GLFW_MOUSE_BUTTON_RIGHT: return "Mouse right";

    case GLFW_KEY_SPACE: return "Space";
    case GLFW_KEY_ESCAPE: return "Escape";
    case GLFW_KEY_ENTER: return "Enter";
    case GLFW_KEY_TAB: return "Tab";
    case GLFW_KEY_BACKSPACE: return "Backspace";
    case GLFW_KEY_INSERT: return "Insert";
    case GLFW_KEY_DELETE: return "Delete";
    case GLFW_KEY_RIGHT: return "Right Arrow";
    case GLFW_KEY_LEFT: return "Left Arrow";
    case GLFW_KEY_DOWN: return "Down Arrow";
    case GLFW_KEY_UP: return "Up Arrow";
    case GLFW_KEY_PAGE_UP: return "Page Up";
    case GLFW_KEY_PAGE_DOWN: return "Page Down";
    case GLFW_KEY_HOME: return "Home";
    case GLFW_KEY_END: return "End";
    case GLFW_KEY_CAPS_LOCK: return "Caps Lock";
    case GLFW_KEY_SCROLL_LOCK: return "Scroll Lock";
    case GLFW_KEY_NUM_LOCK: return "Num Lock";
    case GLFW_KEY_PRINT_SCREEN: return "Print Screen";
    case GLFW_KEY_PAUSE: return "Pause";
    case GLFW_KEY_LEFT_SHIFT: return "Left Shift";
    case GLFW_KEY_LEFT_CONTROL: return "Left Ctrl";
    case GLFW_KEY_LEFT_ALT: return "Left Alt";
    case GLFW_KEY_LEFT_SUPER: return "Left Super";
    case GLFW_KEY_RIGHT_SHIFT: return "Right Shift";
    case GLFW_KEY_RIGHT_CONTROL: return "Right Ctrl";
    case GLFW_KEY_RIGHT_ALT: return "Right Alt";
    case GLFW_KEY_RIGHT_SUPER: return "Right Super";
    case GLFW_KEY_MENU: return "Menu";

    case GLFW_KEY_F1: return "F1"; case GLFW_KEY_F2: return "F2";
    case GLFW_KEY_F3: return "F3"; case GLFW_KEY_F4: return "F4";
    case GLFW_KEY_F5: return "F5"; case GLFW_KEY_F6: return "F6";
    case GLFW_KEY_F7: return "F7"; case GLFW_KEY_F8: return "F8";
    case GLFW_KEY_F9: return "F9"; case GLFW_KEY_F10: return "F10";
    case GLFW_KEY_F11: return "F11"; case GLFW_KEY_F12: return "F12";

    case GLFW_KEY_KP_0: return "Numpad 0"; case GLFW_KEY_KP_1: return "Numpad 1";
    case GLFW_KEY_KP_2: return "Numpad 2"; case GLFW_KEY_KP_3: return "Numpad 3";
    case GLFW_KEY_KP_4: return "Numpad 4"; case GLFW_KEY_KP_5: return "Numpad 5";
    case GLFW_KEY_KP_6: return "Numpad 6"; case GLFW_KEY_KP_7: return "Numpad 7";
    case GLFW_KEY_KP_8: return "Numpad 8"; case GLFW_KEY_KP_9: return "Numpad 9";
    case GLFW_KEY_KP_DECIMAL: return "Numpad ."; case GLFW_KEY_KP_DIVIDE: return "Numpad /";
    case GLFW_KEY_KP_MULTIPLY: return "Numpad *"; case GLFW_KEY_KP_SUBTRACT: return "Numpad -";
    case GLFW_KEY_KP_ADD: return "Numpad +"; case GLFW_KEY_KP_ENTER: return "Numpad Enter";
    case GLFW_KEY_KP_EQUAL: return "Numpad =";

    case GLFW_KEY_0: return ";";
    case GLFW_KEY_1: return "1";
    case GLFW_KEY_2: return "2";
    case GLFW_KEY_3: return "3";
    case GLFW_KEY_4: return "4";
    case GLFW_KEY_5: return "5";
    case GLFW_KEY_6: return "6";
    case GLFW_KEY_7: return "7";
    case GLFW_KEY_8: return "8";
    case GLFW_KEY_9: return "9";
    case 58: return "0";
    }
    const char* name = glfwGetKeyName(key, 0);
    if (name) return name;
    return "ERROR";
}