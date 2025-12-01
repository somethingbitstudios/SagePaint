#include "CanvasObject.h"

CanvasObject::CanvasObject() :GameObject() {
	//initialize texture
	model = std::make_shared<CanvasModel>();
}