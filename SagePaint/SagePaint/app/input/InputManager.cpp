#include "InputManager.h"
#include "InputFunctions.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
//#include "../CanvasManager.h"
InputManager::InputManager() {

}
InputManager::~InputManager() {

}
void InputManager::ShowUI() {
    ImGui::Text("Input binding:"); 
    ImGui::SameLine();
    if (ImGui::Button("Save")) {

    }
    ImGui::SameLine();
    if (ImGui::Button("Revert changes")) {

    }

    for (int k = 0; k < inputFunctions.size(); k++) {
        ImGui::Text(inputFunctions[k].name.c_str());
        //ImGui::SameLine();
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 400.0f);
        ImGui::TextColored(ImVec4(0.5,0,0.5,1), inputFunctions[k].description.c_str());
        
        ImGui::Text("bound to:");
        ImGui::Text("   On Down:");
        for (int i = 0; i < inputFunctions[k].key.size(); i++) {
            ImGui::TextColored(ImVec4(0.8,0,0.8,1),GetGLFWKeyName(inputFunctions[k].key[i]));
        }
        ImGui::Text("   On hold:");
        for (int i = 0; i < inputFunctions[k].keyHold.size(); i++) {
            ImGui::TextColored(ImVec4(0.8, 0, 0.8, 1), GetGLFWKeyName(inputFunctions[k].keyHold[i]));
        }
        ImGui::Text("   On Up:");
        for (int i = 0; i < inputFunctions[k].keyUp.size(); i++) {
            ImGui::TextColored(ImVec4(0.8, 0, 0.8, 1), GetGLFWKeyName(inputFunctions[k].keyUp[i]));
        }
        //inputFunctions[k].
    }
    /*
    ImGui::Text("Showing: "); ImGui::SameLine();
    
    static int show_mode = 0;
    auto getShowText = []() -> const char* {
        switch (show_mode) {
        case 2: return "Bound keys";
        case 1: return "Unbound keys";
        default: return "All";
        }
        };

    if (ImGui::Button(getShowText())) {
        show_mode++;
        show_mode %= 3;
    }

	for (int i = 0; i < 349; i++) {
        const char* c = GetGLFWKeyName(i);
        if (c != nullptr) {
            ImGui::Text(c);
            KeyAction ia = keyMap.GetKey(i);
            
            if ((!ia.IsAssignedTo() && show_mode<2) || show_mode!=1) {
                ImGui::SameLine(); ImGui::Text("OnDown:");
                for (int j = 0; j < ia.funcs.size(); j++) {
                    ImGui::Text(());
                    ia.funcs[j].func();
                }
            }
        }
		}
        */
}
void InputManager::Init() {
	SetContext(KEY_CONTEXT_UI);//WARN: set to ui instead of default for testing only
	InitInputFunctions();

	keyMap.Default(0);
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
    const char* name = glfwGetKeyName(key, 0);
    if (name) return name;

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

    default: return nullptr; // Unmapped or empty gap in GLFW definitions
    }
}