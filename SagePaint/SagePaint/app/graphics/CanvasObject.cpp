#include "CanvasObject.h"
#include "shortcuts.h"

CanvasObject::CanvasObject() :GameObject() {
	layers = std::make_shared<std::vector<LayerPtr>>();
	//initialize canvas
	//image = std::make_shared<Image>(640,360);//WARN: might be extra and not needed!
	//image->texture[0] = 255;
	//image->texture[1] = 0;
	//image->texture[2] = 0;
	//image->texture[3] = 255;

	//layers.push_back(image);
	model = std::make_shared<CanvasModel>();
	//model->SetImage(image);
	model->SetLayerVector(layers);
	//scast(CanvasModel, model)->SetImage(image); alternative
}
void CanvasObject::LoadImageSync(std::string path) {
	image = std::make_shared<Image>(path);
	model->SetImage(image);
	//scast(CanvasModel, model)->SetImage(image);//alert canvas that image changed
}
void CanvasObject::LoadImageSync(ImagePtr i) {
	image = i;
	layers->push_back(std::make_shared<Layer>(image));
	selectedLayer = layers->size()-1;
	//model->SetImage(i);
	model->selected_layer = selectedLayer;
	model->InitLayer();
	//scast(CanvasModel, model)->SetImage(i);//alert canvas that image changed
}

CanvasObject::~CanvasObject() {
	DLOG("Canvas start deletion")
	
}
void CanvasObject::Changed() {
	model->Changed();
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
void CanvasObject::SetZoom(float zoom,float forceNearestThreshold) {

	scale.x = (float)image->width * zoom;
	scale.y = (float)image->height * zoom;
	model->SetZoom(zoom,forceNearestThreshold);
}

void CanvasObject::SwapLayerUp(int index) {
	if (index >= 0 && index < layers->size()-1) {
		LayerPtr temp = (*layers)[index+1];
		(*layers)[index+1] = (*layers)[index];
		(*layers)[index] = temp;

		if (index == selectedLayer)
			selectedLayer++;
		else if (index + 1 == selectedLayer)
			selectedLayer--;

		model->selected_layer = selectedLayer;
		model->SwapLayerUp(index);
		//Changed();
	}

	//Changed();//TODO:remove, only debug?
	}

void CanvasObject::SwapLayerDown(int index) {
	if (index > 0 && index < layers->size() ) {
		LayerPtr temp = (*layers)[index-1];
		(*layers)[index-1] = (*layers)[index];
		(*layers)[index] = temp;

		if (index == selectedLayer)
			selectedLayer--;
		else if (index - 1 == selectedLayer)
			selectedLayer++;


		model->selected_layer = selectedLayer;
		model->SwapLayerDown(index);
		//Changed();
	}

	//Changed();//TODO: -||-
}
