#include "InputMap.h"
#include <GLFW/glfw3.h>
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

	//adds empty values for all keys, at least 32-348 for keys, and add some for mouse at 1,2 maybe
	for (int i = 32; i < 349;i++) {
		
		keyMap[i] = KeyAction{};
	}
	keyMap[0] = KeyAction{};//mouse left
	keyMap[1] = KeyAction{};//mouse middle
	keyMap[2] = KeyAction{};//mouse right
}
bool InputMap::Action(int key, int action, int mods) {
	DLOG("[INPUT] Key: " << key << " Action: " << action)
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



void InputMap::Default(int defaultType) {
	if (keyMap.size() == 0) {
		Init();
	}

	

	//is this the best way?
	switch (defaultType) {
	default:

		keyMap[0].mode = true; //not additive
		
		KeyFunction a;
		a.priority = 0;
		a.context = KEY_CONTEXT_DEFAULT;
		a.func = []() { // consider modifying to be able to directly call a function from elsewhere 
			DLOG("   Pressed mouse button left")
			};
		keyMap[0].funcs.emplace_back(a);
		


		a.priority = 0;
		a.context = KEY_CONTEXT_UI;
		a.func = []() { // consider modifying to be able to directly call a function from elsewhere 
			DLOG("   Pressed mouse button left UI override")
			};
		keyMap[0].funcs.emplace_back(a);
		

		a.priority = 0;
		a.context = KEY_CONTEXT_DEFAULT;
		a.func = []() { // consider modifying to be able to directly call a function from elsewhere 
			DLOG("   Released mouse button left")
			};
		keyMap[0].funcsRelease.emplace_back(a);



		a.priority = 0;
		a.context = KEY_CONTEXT_DEFAULT;
		a.func = []() { // consider modifying to be able to directly call a function from elsewhere 
			DLOG("   Holding mouse button left                                    x")
			};
		keyMap[0].funcsHold.emplace_back(a);

		a.priority = 0;
		a.context = KEY_CONTEXT_DEFAULT;
		a.func = []() { // consider modifying to be able to directly call a function from elsewhere 
			DLOG("   Holding A                                                 y  y")
			};
		keyMap[GLFW_KEY_A].funcsHold.emplace_back(a);




		keyMap[1].mode = true;
		a.priority = 0;
		a.context = KEY_CONTEXT_DEFAULT;
		a.func = []() { // consider modifying to be able to directly call a function from elsewhere 
			DLOG("   Pressed mouse button right")
			};
		keyMap[1].funcs.emplace_back(a);
		
		
		break;
	}
}