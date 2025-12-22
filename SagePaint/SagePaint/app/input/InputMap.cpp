#include "InputMap.h"
#include <GLFW/glfw3.h>
#include "InputFunctions.h"



Key_Context_Enum kc;//WARN:change to an actually good location
void InputMap::SetContext(Key_Context_Enum keyContext) {
	kc = keyContext;
}


InputMap::InputMap() {
	//no init is supposed to be here, to avoid unnecessary calls, because a later call to Init, Default or manually adding is expected
}

InputMap::InputMap(int defaultType) {
	Default(defaultType);
}

void InputMap::Init() {
	//inputFunctions[0] = { Click,   "Click",   "does click",        1 };
	//inputFunctions[1] = { ClickUI, "ClickUI", "does click in UI",  2 };

	//adds empty values for all keys, at least 32-348 for keys, and add some for mouse at 1,2 maybe
	for (int i = 32; i < 349;i++) {
		
		keyMap[i] = KeyAction{};
	}
	keyMap[0] = KeyAction{};//mouse left
	keyMap[1] = KeyAction{};//mouse middle
	keyMap[2] = KeyAction{};//mouse right
}
bool InputMap::Action(int key, int action, int mods) {
	//DLOG("[INPUT] Key: " << key << " Action: " << action)
		if (action == 1) {//pressed
			if (keyMap[key].mode) { //execute all funcs that are in context
				int index = -1;
				int priority = -999;
				for (int i = 0; i < keyMap[key].funcs.size(); i++) {
					if (keyMap[key].funcs[i].context == KEY_CONTEXT_DEFAULT || keyMap[key].funcs[i].context == kc) {
						if (keyMap[key].funcs[i].priority >= priority) {
							index = i;
							priority = keyMap[key].funcs[i].priority;
						}
					}
				}
				if (index >= 0) {
					keyMap[key].funcs[index].func();
				}
				
			}
			else { //execute the func with highest priority in context
				for (int i = 0; i < keyMap[key].funcs.size(); i++) {
					if (keyMap[key].funcs[i].context == KEY_CONTEXT_DEFAULT || keyMap[key].funcs[i].context == kc) {
						keyMap[key].funcs[i].func();
					}
				}
			}


		}
		else if (action == 0) {
			if (keyMap[key].mode) { //execute all funcs that are in context
				int index = -1;
				int priority = -999;
				for (int i = 0; i < keyMap[key].funcsRelease.size(); i++) {
					if (keyMap[key].funcsRelease[i].context == KEY_CONTEXT_DEFAULT || keyMap[key].funcsRelease[i].context == kc) {
						if (keyMap[key].funcsRelease[i].priority >= priority) {
							index = i;
							priority = keyMap[key].funcsRelease[i].priority;
						}
					}
				}
				if (index >= 0) {
					keyMap[key].funcsRelease[index].func();
				}
			}
			else { //execute the func with highest priority in context
				for (int i = 0; i < keyMap[key].funcsRelease.size(); i++) {
					if (keyMap[key].funcsRelease[i].context == KEY_CONTEXT_DEFAULT || keyMap[key].funcsRelease[i].context == kc) {
						keyMap[key].funcsRelease[i].func();
					}
				}
			}
		}
		else if (action==3) {//held (each frame including the pressed one)
			if (keyMap[key].mode) { //execute all funcs that are in context
				int index = -1;
				int priority = -999;
				for (int i = 0; i < keyMap[key].funcsHold.size(); i++) {
					if (keyMap[key].funcsHold[i].context == KEY_CONTEXT_DEFAULT || keyMap[key].funcsHold[i].context == kc) {
						if (keyMap[key].funcsHold[i].priority >= priority) {
							index = i;
							priority = keyMap[key].funcsHold[i].priority;
						}
					}
				}
				if (index >= 0) {
					keyMap[key].funcsHold[index].func();
				}
			}
			else { //execute the func with highest priority in context
				for (int i = 0; i < keyMap[key].funcsHold.size(); i++) {
					if (keyMap[key].funcsHold[i].context == KEY_CONTEXT_DEFAULT || keyMap[key].funcsHold[i].context == kc) {
						keyMap[key].funcsHold[i].func();
					}
				}
			}

		}
		
	
	
	return true;//true -> ok'd, false -> blocked
}
//will be better for the default config because it's faster
void InputMap::InitKeyFunction(KeyFunction *kf,int priority,Key_Context_Enum context,int functionNumber) {
	kf->priority = priority;
	kf->context = context;

	if (functionNumber >= 0 && functionNumber < inputFunctions.size()) {
		kf->func = inputFunctions[functionNumber].func;
	}
}
void InputMap::InitKeyFunction(KeyFunction* kf, int priority, Key_Context_Enum context, std::string functionName) {

	

	kf->priority = priority;
	kf->context = context;
	int size = inputFunctions.size();
	for (int i = 0; i < size; i++) {
		if (inputFunctions[i].name == functionName) {
			kf->func = inputFunctions[i].func;
		}
	}
}
void InputMap::Default(int defaultType) {
	if (keyMap.size() == 0) {
		Init();
	}

	
	KeyFunction a;
	//is this the best way?
	switch (defaultType) {
	default:

		keyMap[GLFW_MOUSE_BUTTON_LEFT].mode = true; //not additive, override
		
		InitKeyFunction(&a, 0, KEY_CONTEXT_DEFAULT, "Draw");
		keyMap[GLFW_MOUSE_BUTTON_LEFT].funcs.emplace_back(a);

		/*
		InitKeyFunction(&a, 0, KEY_CONTEXT_UI, "ClickUI");
		keyMap[GLFW_MOUSE_BUTTON_LEFT].funcs.emplace_back(a);
		*/
		

		
		break;
	}
}