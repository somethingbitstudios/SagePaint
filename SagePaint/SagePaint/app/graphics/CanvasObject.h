#pragma once
#include "GameObject.h"
#include "./drawables/CanvasModel.h"
#include "../file/Image.h"
class CanvasObject : public GameObject {
public:
	CanvasObject();
	~CanvasObject();
	void LoadImage(std::string path);
	void LoadImage(ImagePtr i);
	ImagePtr image;

};
typedef std::shared_ptr<CanvasObject> CanvasObjectPtr;