#pragma once
#include "debug.h"
#include <unordered_map>
#include <functional>
#include "InputMap.h"
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float2.hpp>

//ok so it works like:
/* WARN / TODO: maybe this would be better as a static class?
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
	static void Init();
	static void LoadInputMap(std::string path); //json?
	static void UpdateCursorPos(GLFWwindow* window);
	static void SetCursorPos(float x,float y);
	static void Input(int key, int action, int mods);
	static void ProcessHeld();
	static void SetContext(Key_Context_Enum keyContext);
	static inline glm::vec2 GetCursorPos() { return glm::vec2 { cursorX.load(), cursorY.load()}; }
	static inline glm::vec2 GetCursorPosDelta() { return glm::vec2{ cursorX-cursorXold, cursorY-cursorYold }; }

	static inline float GetCursorX() { return cursorX; }
	static inline float GetCursorXDelta() { return cursorX-cursorXold; }
	static inline float GetCursorY() { return cursorY; }
	static inline float GetCursorYDelta() { return cursorY-cursorYold; }
private:
	static std::atomic<float> cursorX, cursorY;//WARN: not sure atomic is neccesary, test later
	static std::atomic<float> cursorXold, cursorYold;
	static std::vector<int> keyHeld;//has data for held keys
	static InputMap keyMap; //have an extra immutable keymap for menus so no softlocks happen, it has no shortcuts
	//escape not rebindable?
};

typedef std::shared_ptr<InputManager> InputManagerPtr;