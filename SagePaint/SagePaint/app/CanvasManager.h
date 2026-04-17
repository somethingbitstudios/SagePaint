#pragma once
#include "graphics/CanvasObject.h"
#include "input/InputManager.h"
#include <glm/ext/vector_int2.hpp>
enum CanvasFilteringMode {
	NEAREST,
	BILINEAR,
	ADVANCED
};
class CanvasManager {
public:
	static unsigned char transparent[4];
	static unsigned char erasePreviewColor[4];

	static void LineStart();
	//static void PencilDrawBasic();
	static void Drag();
	static void Init();
	static void UpdateRelativeCursorPos();
	static inline glm::ivec2 GetRelativeCursorPos() { return relativeCursorPos; }
	static inline glm::ivec2 GetLastRelativeCursorPos() { return lastRelativeCursorPos; }
	static void OpacityChanged(unsigned int i);
	static void UpdateColor();
	static CanvasObjectPtr GetCanvas();
	static void ZoomOut();
	static void ZoomIn();
	static void Clear();
	static void ResChange(unsigned int rX, unsigned int rY);
	static void Crop(int x, int y, int oX, int oY);
	static void ChangeBlendMode(unsigned int i);
	static void Export(std::string path);
	static CanvasObjectPtr obj;
	static unsigned char color[4];
	static float colorFloat[4];

	static float backgroundColor[4];

	static int initialWidth;
	static int initialHeight;
	static int canvasFilteringInt;
	static CanvasFilteringMode canvasFiltering;

	static float zoom;
	static float zoomRateAdditiveMicro;
	static float zoomRateAdditiveMacro;
	static float zoomRateMultiplicative;

	static float scalingNonIntUseLinearThreshold;
	static bool zoomType;//false -> mul, true -> add
	static bool erase;
	static glm::ivec2 relativeCursorPos;
	static glm::ivec2 lastRelativeCursorPos;
};
//typedef std::shared_ptr<CanvasManager> CanvasManagerPtr;//not needed?