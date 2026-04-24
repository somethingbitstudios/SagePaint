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
	keyMap.clear();
	//adds empty values for all keys, at least 32-348 for keys, and add some for mouse at 1,2 maybe
	for (int j = 0; j < 8000; j += 1000) {
		for (int i = 32; i < 349; i++) {

			keyMap[i+j] = KeyAction{};
		}
		keyMap[0+j] = KeyAction{};//mouse left
		keyMap[1+j] = KeyAction{};//mouse middle
		keyMap[2+j] = KeyAction{};//mouse right
	}
		
	
}
bool InputMap::Action(int key, int action) {
		//DLOG("[INPUT] Key: " << key << " mods: " << mods)
		//DLOG("[IN]: "<< key)
		if (action == 1) {//pressed
			if (keyMap[key].mode) { //execute all funcs that are in context (ctrl/shift etc.
				int index = -1;
				int priority = -999;
				for (int i = 0; i < keyMap[key].funcs.size(); i++) {
					if ((int)keyMap[key].funcs[i].context == KEY_CONTEXT_DEFAULT/* || keyMap[key].funcs[i].context == kc*/) {
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
					if ((int)keyMap[key].funcs[i].context == KEY_CONTEXT_DEFAULT /* || keyMap[key].funcs[i].context == kc*/) {
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
					if ((int)keyMap[key].funcsRelease[i].context == KEY_CONTEXT_DEFAULT/* || keyMap[key].funcsRelease[i].context == kc*/) {
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
					if ((int)keyMap[key].funcsRelease[i].context == KEY_CONTEXT_DEFAULT /*|| keyMap[key].funcsRelease[i].context == kc*/) {
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
					if ((int)keyMap[key].funcsHold[i].context == KEY_CONTEXT_DEFAULT /*|| keyMap[key].funcsHold[i].context == kc*/) {
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
					if ((int)keyMap[key].funcsHold[i].context == KEY_CONTEXT_DEFAULT /* || keyMap[key].funcsHold[i].context == kc*/) {
						keyMap[key].funcsHold[i].func();
					}
				}
			}

		}
		
	
	
	return true;//true -> ok'd, false -> blocked
}
KeyAction InputMap::GetKey(int key)
{
	return keyMap[key];
}

//will be better for the default config because it's faster
void InputMap::InitKeyFunction(KeyFunction *kf,int priority,Key_Context_Enum context,int functionNumber) {
	kf->priority = priority;
	kf->context = context;

	if (functionNumber >= 0 && functionNumber < inputFunctions.size()) {
		kf->func = inputFunctions[functionNumber].func;
	}
}
void InputMap::InitKeyFunction(int key,int state,KeyFunction* kf, int priority, Key_Context_Enum context, std::string functionName) {

	

	kf->priority = priority;
	kf->context = context;
	int size = inputFunctions.size();
	for (int i = 0; i < size; i++) {
		if (inputFunctions[i].name == functionName) {
			kf->func = inputFunctions[i].func;
			switch (state) {
			case 0:
				inputFunctions[i].key.push_back(key);
				break;
			case 1:
				inputFunctions[i].keyHold.push_back(key);
				break;
			case 2:
				inputFunctions[i].keyUp.push_back(key);
				break;
			}
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
		
		keyMap[GLFW_MOUSE_BUTTON_LEFT].mode = false; //true -> not additive, override = specific actions prevent default behaviour of PointerX
		
		InitKeyFunction(GLFW_MOUSE_BUTTON_LEFT,0,&a, 0, KEY_CONTEXT_DEFAULT, "PointerDown");
		keyMap[GLFW_MOUSE_BUTTON_LEFT].funcs.emplace_back(a);
		InitKeyFunction(GLFW_MOUSE_BUTTON_LEFT, 1, &a, 0, KEY_CONTEXT_DEFAULT, "Pointer");
		keyMap[GLFW_MOUSE_BUTTON_LEFT].funcsHold.emplace_back(a);
		InitKeyFunction(GLFW_MOUSE_BUTTON_LEFT, 2, &a, 0, KEY_CONTEXT_DEFAULT, "PointerUp");
		keyMap[GLFW_MOUSE_BUTTON_LEFT].funcsRelease.emplace_back(a);

		
		keyMap[GLFW_MOUSE_BUTTON_RIGHT].mode = false; //not additive, override

		InitKeyFunction(GLFW_MOUSE_BUTTON_RIGHT, 1, &a, 0, KEY_CONTEXT_DEFAULT, "Drag");
		keyMap[GLFW_MOUSE_BUTTON_RIGHT].funcsHold.emplace_back(a);
		InitKeyFunction(GLFW_MOUSE_BUTTON_RIGHT, 2, &a, 0, KEY_CONTEXT_DEFAULT, "DragUp");
		keyMap[GLFW_MOUSE_BUTTON_RIGHT].funcsRelease.emplace_back(a);
		/*
		GLFW_KEY_LEFT_CONTROL
		GLFW_KEY_RIGHT_CONTROL
		GLFW_KEY_LEFT_SHIFT
		GLFW_KEY_RIGHT_SHIFT
		GLFW_KEY_LEFT_ALT
		GLFW_KEY_RIGHT_ALT
		*/

		keyMap[GLFW_KEY_W].mode = false; //not additive, override

		InitKeyFunction(GLFW_KEY_W, 0, &a, 0, KEY_CONTEXT_DEFAULT, "ZoomOut");
		keyMap[GLFW_KEY_W].funcs.emplace_back(a);

		keyMap[GLFW_KEY_S].mode = true; //not additive, override

		InitKeyFunction(GLFW_KEY_S, 0, &a, 0, KEY_CONTEXT_DEFAULT, "ZoomIn");
		keyMap[GLFW_KEY_S].funcs.emplace_back(a);

		
		break;
	}
}