#pragma once
#include "GameObject.h"
#include "./drawables/CanvasModel.h"
#include "../file/Image.h"
class CanvasObject : public GameObject {
public:
	CanvasObject();
	~CanvasObject();
	void LoadImageSync(std::string path);
	void LoadImageSync(ImagePtr i);
	void Changed();
	void SetZoom(float zoom, float forceNearestThreshold);
	CanvasModelPtr model;


	//TODO: maintain layers here (?), then send as a & reference parameter to model
	//TODO: Make a Layer class with bool visible, blend type etc.
	ImagePtr image;

	void Draw() override;
	int GetType() const override { return 1; }
};
typedef std::shared_ptr<CanvasObject> CanvasObjectPtr;