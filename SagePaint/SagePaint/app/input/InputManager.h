#pragma once
#include "debug.h"
#include <unordered_map>
#include <functional>
#include "InputMap.h"
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>

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
	void UpdateCursorPos(GLFWwindow* window);

	void Input(int key, int action, int mods);
	void ProcessHeld();
	void SetContext(Key_Context_Enum keyContext);
	glm::vec2 GetCursorPos() { return glm::vec2 { cursorX, cursorY }; }
	glm::vec2 GetCursorPosDelta() { return glm::vec2{ cursorX-cursorXold, cursorY-cursorYold }; }

	float GetCursorX() { return cursorX; }
	float GetCursorXDelta() { return cursorX-cursorXold; }
	float GetCursorY() { return cursorY; }
	float GetCursorYDelta() { return cursorY-cursorYold; }
private:
	float cursorX, cursorY;
	float cursorXold, cursorYold;
	std::vector<int> keyHeld;//has data for held keys
	InputMap keyMap; //have an extra immutable keymap for menus so no softlocks happen, it has no shortcuts
	//escape not rebindable?
};

typedef std::shared_ptr<InputManager> InputManagerPtr;