#pragma once
#include "debug.h"
#include <unordered_map>
#include <functional>
enum Key_Context_Enum {
	DEFAULT = 0,

	ANIMATION = 1
};


struct KeyFunction {
	std::function<void(int key, int action, int mods)> func; //might not need the params
	Key_Context_Enum context;//maybe not the best? meh
};
struct KeyAction {
	bool mode;//ADD functionality or OVERRIDE the function? 
	//the first func is the default, accesible at all times with ADD, and disabled when in context and OVERRIDE mode
	std::vector<KeyFunction> funcs;
};
class InputMap {
private:
	std::unordered_map<int, KeyAction> keyMap;
public:
	InputMap(); //this one is for 'no init'
	InputMap(int defaultType);//this one calls default

	void Init(); //fills the map with keyactions for each key (with an empty vector 'funcs')

	//research what the best way to do default is, when it needs to be part of the program, not some default.kmap file
	void Default(int defaultType /*my, krita-like, no shortcut version(needs to be 0) etc...*/);//calls init() if map is empty, then fills the keymap with default values

	bool Action(int key, int action, int mods); //call KeyFunctions etc etc
};
typedef std::shared_ptr<InputMap> InputMapPtr;

