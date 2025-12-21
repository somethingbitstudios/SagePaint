#include "InputManager.h"

InputManager::InputManager() {
}
InputManager::~InputManager() {

}
void InputManager::LoadInputMap(std::string path) {

}
void InputManager::Input(int key, int action, int mods) {
	keyMap.Action(key, action, mods);
}