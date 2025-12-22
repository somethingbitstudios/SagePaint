#pragma once
#include "debug.h"
#include <unordered_map>
#include <functional>
#include "InputMap.h"

//ok so it works like:
/*
INputmanager loads InputMap
InputMap has map where keys(int) are assigned Keyaction
KeyAction has priority (to be able to override binds based on things like what view is open (general vs animation keybinds) )
and also has a vector of functions to be run one after another


maybe a better structure would be to add the context when functions are supposed to run to the functions instead of having separate InputMaps...
actually IMPLEMENT THIS
*/
class InputManager  {
public:
	InputManager();
	~InputManager();
	void LoadInputMap(std::string path); //json?
	void Input(int key, int action, int mods);
	void ProcessHeld();
	void SetContext(Key_Context_Enum keyContext);
private:
	std::vector<int> keyHeld;//has data for held keys
	InputMap keyMap; //have an extra immutable keymap for menus so no softlocks happen, it has no shortcuts
	//escape not rebindable?
};

typedef std::shared_ptr<InputManager> InputManagerPtr;