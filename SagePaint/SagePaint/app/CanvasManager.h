#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
class CanvasManager {
public:
	static void Draw();
	CanvasObjectPtr obj;
	 
	static InputManagerPtr inputManager;
	
};
typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;