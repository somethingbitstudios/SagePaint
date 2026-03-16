#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
class CanvasManager {
public:
	static void LineStart();
	static void PencilDrawBasic();
	static void Drag();
	static void Init();
	static CanvasObjectPtr GetCanvas();
	static void ZoomOut();
	static void ZoomIn();
	static CanvasObjectPtr obj; 
	static float color[4];
	static float zoom;
	static float zoomRateAdditiveMicro;
	static float zoomRateAdditiveMacro;
	static float zoomRateMultiplicative;

	static float scalingNonIntUseLinearThreshold;
	static bool zoomType;//false -> mul, true -> add
};
//typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?