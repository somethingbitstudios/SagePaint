#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
class CanvasManager {
public:
	static void Draw();
	static void Drag();
	static void Init();
	static CanvasObjectPtr obj; 
	static InputManagerPtr inputManager;
	
};
typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?