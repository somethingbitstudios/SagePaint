#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
class CanvasManager {
public:
	static void Draw();
	static void Drag();
	static void Init();
	static void ZoomOut();
	static void ZoomIn();
	static CanvasObjectPtr obj; 
	static InputManagerPtr inputManager;
	static float zoom;
};
typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?