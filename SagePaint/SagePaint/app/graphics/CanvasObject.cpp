#include "CanvasObject.h"
#include "shortcuts.h"

CanvasObject::CanvasObject() :GameObject() {
	//initialize canvas
	image = std::make_shared<Image>(640,360);
	model = std::make_shared<CanvasModel>();
	model->SetImage(image);
	//scast(CanvasModel, model)->SetImage(image); alternative
}
void CanvasObject::LoadImage(std::string path) {
	image = std::make_shared<Image>(path);
	model->SetImage(image);
	//scast(CanvasModel, model)->SetImage(image);//alert canvas that image changed
}
void CanvasObject::LoadImage(ImagePtr i) {
	model->SetImage(i);
	//scast(CanvasModel, model)->SetImage(i);//alert canvas that image changed
}

CanvasObject::~CanvasObject() {
	DLOG("Canvas start deletion")
	//initialize texture

	image.~shared_ptr();
	model.~shared_ptr();
}