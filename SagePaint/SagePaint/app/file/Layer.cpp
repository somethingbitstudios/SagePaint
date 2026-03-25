#include "Layer.h"

unsigned int Layer::layerCount = 0;

Layer::Layer() {
	name = "Layer" + layerCount++;
	visible = true;
	opacity = 1.0f;
	blend = BLEND_Normal;
	textureId = 0;
}
Layer::~Layer() {

}
Layer::Layer(ImagePtr i) {
	name = "Layer" + layerCount++;
	visible = true;
	opacity = 1.0f;
	blend = BLEND_Normal;
	textureId = 0;
	SetImage(i);//in case processing needs to happen, funnel all to SetImage
}
void Layer::SetImage(ImagePtr i) {
	image = i;
}