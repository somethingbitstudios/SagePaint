#include "CanvasObject.h"


CanvasObject::CanvasObject() :GameObject() {
	//initialize texture
	
	image = std::make_shared<Image>("C:/temp/test.png");
	CanvasModelPtr model_tmp = std::make_shared<CanvasModel>();
	model_tmp->SetImage(image);
	model = model_tmp;
}
void CanvasObject::LoadImage(std::string path) {
	image->LoadImage(path);
	model->Changed();//alert canvas that image changed
}
CanvasObject::~CanvasObject() {
	DLOG("Canvas start deletion")
	//initialize texture

	image.~shared_ptr();
	model.~shared_ptr();
}