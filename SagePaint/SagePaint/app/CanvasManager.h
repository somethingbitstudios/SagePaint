#pragma once
#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
#include <glm/ext/vector_int2.hpp>
class CanvasManager {
public:
	static void LineStart();
	//static void PencilDrawBasic();
	static void Drag();
	static void Init();
	static void UpdateRelativeCursorPos();
	static inline glm::ivec2 GetRelativeCursorPos() { return relativeCursorPos; }
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

	static glm::ivec2 relativeCursorPos;
};
//typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?