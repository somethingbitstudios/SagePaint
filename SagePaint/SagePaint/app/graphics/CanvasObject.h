#pragma once
#include "GameObject.h"
#include "./drawables/CanvasModel.h"
class CanvasObject : public GameObject {
public:
	CanvasObject();
};
typedef std::shared_ptr<CanvasObject> CanvasObjectPtr;