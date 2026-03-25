#pragma once
#include "GameObject.h"
#include "./drawables/CanvasModel.h"
#include "../file/Image.h"
#include <vector>
#include "../file/Layer.h"

class CanvasObject : public GameObject {
public:
	void Draw() override;
	int GetType() const override { return GO_CANVAS; }

	CanvasObject();
	~CanvasObject();
	void LoadImageSync(std::string path);
	void LoadImageSync(ImagePtr i);
	void Changed();
	void SetZoom(float zoom, float forceNearestThreshold);
	CanvasModelPtr model;


	ImagePtr image;

	//TODO: maintain layers here (?), then send as a & reference parameter to model
	//TODO: Make a Layer class with bool visible, blend type etc.
	void SwapLayerUp(int index);
	void SwapLayerDown(int index);

	std::shared_ptr<std::vector<LayerPtr>> layers;
	unsigned int selectedLayer;
};
typedef std::shared_ptr<CanvasObject> CanvasObjectPtr;