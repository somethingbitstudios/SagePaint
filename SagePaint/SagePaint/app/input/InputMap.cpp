#include "InputMap.h"

InputMap::InputMap() {
	//no init is supposed to be here, to avoid unnecessary calls, because a later call to Init, Default or manually adding is expected
}
InputMap::InputMap(int defaultType) {
	Default(defaultType);
}
void InputMap::Init() {
	//adds empty values for all keys, at least ~360 for some reason?
}
bool InputMap::Action(int key, int action, int mods) {
	return true;//true -> ok'd, false -> blocked
}



void InputMap::Default(int defaultType) {
	if (keyMap.size() == 0) {
		Init();
	}


	//is this the best way?
	switch (defaultType) {
	default:

		break;
	}
}