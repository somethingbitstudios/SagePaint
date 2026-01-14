#include "InputManager.h"
#include "InputFunctions.h"
InputManager::InputManager() {

}
InputManager::~InputManager() {

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
void InputManager :: ProcessHeld() {
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