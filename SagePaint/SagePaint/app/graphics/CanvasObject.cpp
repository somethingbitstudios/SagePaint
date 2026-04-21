#include "CanvasObject.h"
#include "shortcuts.h"
#include <algorithm>

CanvasObject::CanvasObject() :GameObject() {
	resX = 640;
	resY = 400;
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
//TODO:make useful
void CanvasObject::AddLayer(std::string path) {
	image = std::make_shared<Image>(path);
	layers->push_back(std::make_shared<Layer>(image));
	selectedLayer = layers->size() - 1;

	model->selected_layer = selectedLayer;
	model->InitLayer(layers->size() - 1);
}
void CanvasObject::AddLayer(ImagePtr i) {
	image = i;
	layers->push_back(std::make_shared<Layer>(image));
	selectedLayer = layers->size()-1;
	//model->SetImage(i);
	model->selected_layer = selectedLayer;
	model->InitLayer(layers->size()-1);
	//scast(CanvasModel, model)->SetImage(i);//alert canvas that image changed
}
void CanvasObject::AddLayer(LayerPtr l){
	layers->push_back(l);
	model->InitLayer(layers->size() - 1);


}
void CanvasObject::ChangeBlendMode(unsigned int i)
{
	BLEND_Type bl = (BLEND_Type)(*layers)[i]->blendCandidate;
	if ((*layers)[i]->blend != bl) {
		(*layers)[i]->blend = bl;
		model->Changed(i);
	}
	
}
ImagePtr CanvasObject::Export() {
	return model->Export();
}
void CanvasObject::SetSelectedLayer(int i)
{
	if (i < 0)i += layers->size();
	i = std::min((int)layers->size() - 1, std::max(i, 0));
	selectedLayer = i;
	model->selected_layer = i;
}

CanvasObject::~CanvasObject() {
	DLOG("Canvas start deletion")
	
}
void CanvasObject::Changed() {
	model->Changed();
}
void CanvasObject::Changed(unsigned int layer) {
	model->Changed(layer);
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
float zoom1 = 1;
float force1 = 0;
void CanvasObject::SetZoomCached() {
	scale.x = (float)resX * zoom1;
	scale.y = (float)resY * zoom1;
	model->SetZoom(zoom1, force1);

}
void CanvasObject::SetZoom(float zoom,float forceNearestThreshold) {

	scale.x = (float)resX * zoom;
	scale.y = (float)resY * zoom;
	model->SetZoom(zoom,forceNearestThreshold);
	zoom1 = zoom;
	force1 = forceNearestThreshold;
}

void CanvasObject::ResChange(unsigned int rX, unsigned int rY)
{
	resX = rX;
	resY = rY;
	model->ResChange(rX, rY);
}

void CanvasObject::Crop(int x, int y, int oX, int oY) {
	if (x > 0 && y > 0) {
		int lc = layers->size();
		for (int i = 0; i < lc; i++) {
			LayerPtr layer = (*layers)[i];

			ImagePtr image = layer->image;

			unsigned char* new_tex = new unsigned char[x * y * 4];
			for (int dest_y = 0; dest_y < y; dest_y++) {
				for (int dest_x = 0; dest_x < x; dest_x++) {

					int dest_index = (dest_y * x + dest_x) * 4;

					int src_x = dest_x + oX;
					int src_y = dest_y + oY;

					if (src_x >= 0 && src_x < image->width &&
						src_y >= 0 && src_y < image->height) {

						int src_index = (src_y * image->width + src_x) * 4;

						new_tex[dest_index] = image->texture[src_index];
						new_tex[dest_index + 1] = image->texture[src_index + 1];
						new_tex[dest_index + 2] = image->texture[src_index + 2];
						new_tex[dest_index + 3] = image->texture[src_index + 3];
					}
					else {
						new_tex[dest_index] = 0;
						new_tex[dest_index + 1] = 0;
						new_tex[dest_index + 2] = 0;
						new_tex[dest_index + 3] = 0;
					}
				}
			}

			delete[] image->texture;

			image->texture = new_tex;
			image->width = x;
			image->height = y;
		}
		ResChange(x, y);
		SetZoomCached();
		model->SendResizedToGpu();

	}
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
		model->DrawFbo();
		//model->SwapLayerUp(index);
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
		model->DrawFbo();
		//model->SwapLayerDown(index);
		//Changed();
	}

	//Changed();//TODO: -||-
}

void CanvasObject::ToggleVisible(int index) {//TODO: add check to all index based functions
	(*layers)[index]->visible = !(*layers)[index]->visible;
	model->DrawFbo();
}
bool CanvasObject::GetVisible(int index) {//TODO: add check to all index based functions
	return (*layers)[index]->visible;
}
void CanvasObject::Remove(int index) {
	//TODO: don't immediately remove, keep for potential undos!
	
	
	//selectedLayer adjust a) before index -> ignore b)index c)after index
	if (selectedLayer >= index ) {
		if (selectedLayer == 0) {
			if ((*layers).size() <= 1) {// deleting last layer
				selectedLayer = -1;
				model->selected_layer = -1;
			}//else not last layer, problem will solve itself
		
		}
		else {
			selectedLayer--;
			model->selected_layer = selectedLayer;
		}
	}
	//model notify
	model->Discard(index);
	//layers remove
	(*layers).erase((*layers).begin() + index);
	//(*layers)[index]->visible = !(*layers)[index]->visible;
	model->DrawFbo();
}
void CanvasObject::AddLayer() {
	AddLayer(std::make_shared<Image>(resX, resY));
}