#include "CanvasObject.h"
#include "shortcuts.h"

CanvasObject::CanvasObject() :GameObject() {
	//initialize canvas
	image = std::make_shared<Image>(640,360);//WARN: might be extra and not needed!
	model = std::make_shared<CanvasModel>();
	model->SetImage(image);
	//scast(CanvasModel, model)->SetImage(image); alternative
}
void CanvasObject::LoadImageSync(std::string path) {
	image = std::make_shared<Image>(path);
	model->SetImage(image);
	//scast(CanvasModel, model)->SetImage(image);//alert canvas that image changed
}
void CanvasObject::LoadImageSync(ImagePtr i) {
	image = i;
	model->SetImage(i);
	//scast(CanvasModel, model)->SetImage(i);//alert canvas that image changed
}

CanvasObject::~CanvasObject() {
	DLOG("Canvas start deletion")
	
}


void CanvasObject::Draw() {
	glm::mat4 m, p;
	m = glm::mat4(1.0f);

	m = glm::translate(m, pos);


	m = glm::rotate(m, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, scale);

	p = glm::ortho(0.0f, Screen_width, Screen_height, 0.f, -1.0f, 1.0f);

	model->Draw(m, p);
}