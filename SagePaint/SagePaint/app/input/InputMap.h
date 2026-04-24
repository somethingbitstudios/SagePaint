#pragma once
#include "debug.h"
#include <unordered_map>
#include <functional>
//#include "InputManager.h"
enum Key_Context_Enum {
	
	KEY_CONTEXT_DEFAULT = 0,
	KEY_CONTEXT_SHIFT = 1,
	KEY_CONTEXT_CTRL = 2,
	KEY_CONTEXT_CTRL_SHIFT = 3,
	KEY_CONTEXT_ALT = 4,
	KEY_CONTEXT_ALT_SHIFT = 5,
	KEY_CONTEXT_CTRL_ALT = 6,
	KEY_CONTEXT_CTRL_ALT_SHIFT = 7

};


struct KeyFunction {

	void (*func)(); 
	Key_Context_Enum context;//maybe not the best? meh ( using a longs individual bits would allow for 64 contexts at once )
	int priority;//might not use
	
};
struct KeyAction {
	bool mode=false;//ADD functionality or OVERRIDE the function if ? 
	//the first func is the default, accesible at all times with ADD, and disabled when in context and OVERRIDE mode
	std::vector<KeyFunction> funcs;
	std::vector<KeyFunction> funcsHold;
	std::vector<KeyFunction> funcsRelease;
	bool IsAssignedTo()
	{
		if (funcs.size() > 0 || funcsHold.size() > 0 || funcsRelease.size() > 0) {
			return true;
		}
			return false;
	}
};
class InputMap {
public:
	std::unordered_map<int, KeyAction> keyMap; // not sure if an array would not be better after all, but it depends on the amount of used keys I suppose, this saves space
	
	//static Key_Context_Enum keyContext;

	InputMap(); //this one is for 'no init'
	InputMap(int defaultType);//this one calls default

	void Init(); //fills the map with keyactions for each key (with an empty vector 'funcs')
	
	void InitKeyFunction(KeyFunction* kf, int priority, Key_Context_Enum context, int functionNumber);
	void InitKeyFunction(int key,int state,KeyFunction* kf, int priority, Key_Context_Enum context, std::string functionName);

	//research what the best way to do default is, when it needs to be part of the program, not some default.kmap file
	void Default(int defaultType /*my, krita-like, no shortcut version(needs to be 0) etc...*/);//calls init() if map is empty, then fills the keymap with default values
	
	bool Action(int key, int action); //call KeyFunctions etc etc
	KeyAction GetKey(int key);
	static void SetContext(Key_Context_Enum keyContext);
};
typedef std::shared_ptr<InputMap> InputMapPtr;

