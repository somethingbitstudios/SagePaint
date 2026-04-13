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
	void AddLayer(std::string path);
	void AddLayer(ImagePtr i);
	void AddLayer(LayerPtr l);
	void SetSelectedLayer(int i);
	void Changed();
	void Changed(unsigned int layer);
	void SetZoom(float zoom, float forceNearestThreshold);
	void ResChange(unsigned int rX, unsigned int rY);
	CanvasModelPtr model;

	void ChangeBlendMode(unsigned int i);
	//deprecated
	ImagePtr image;

	//TODO: maintain layers here (?), then send as a & reference parameter to model
	//TODO: Make a Layer class with bool visible, blend type etc.
	void SwapLayerUp(int index);
	void SwapLayerDown(int index);
	
	void ToggleVisible(int index);
	bool GetVisible(int index);
	void Remove(int index);

	void AddLayer();

	std::shared_ptr<std::vector<LayerPtr>> layers;
	int selectedLayer;
	unsigned int resX, resY;
};
typedef std::shared_ptr<CanvasObject> CanvasObjectPtr;