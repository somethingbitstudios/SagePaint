#include "InputManager.h"

InputManager::InputManager() {
	SetContext(KEY_CONTEXT_UI);//WARN: set to ui instead of default for testing only
}
InputManager::~InputManager() {

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