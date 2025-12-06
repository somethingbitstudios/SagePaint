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
	ImagePtr image;
	void Draw() override;

};
typedef std::shared_ptr<CanvasObject> CanvasObjectPtr;