#pragma once
#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
#include <glm/ext/vector_int2.hpp>
class CanvasManager {
public:
	static unsigned char transparent[4];

	static void LineStart();
	//static void PencilDrawBasic();
	static void Drag();
	static void Init();
	static void UpdateRelativeCursorPos();
	static inline glm::ivec2 GetRelativeCursorPos() { return relativeCursorPos; }
	static inline glm::ivec2 GetLastRelativeCursorPos() { return lastRelativeCursorPos; }
	static CanvasObjectPtr GetCanvas();
	static void ZoomOut();
	static void ZoomIn();
	static void Clear();
	static CanvasObjectPtr obj; 
	static float color[4];
	static float zoom;
	static float zoomRateAdditiveMicro;
	static float zoomRateAdditiveMacro;
	static float zoomRateMultiplicative;

	static float scalingNonIntUseLinearThreshold;
	static bool zoomType;//false -> mul, true -> add

	static glm::ivec2 relativeCursorPos;
	static glm::ivec2 lastRelativeCursorPos;
};
//typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?