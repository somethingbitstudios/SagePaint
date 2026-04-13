#include "Layer.h"

unsigned int Layer::layerCount = 0;

void Layer::Init() {

	name = "Layer " + std::to_string(layerCount++);
	visible = true;
	opacity = 1.0f;
	blendCandidate = 0;
	blend = BLEND_Normal;
	textureId = 0;
	id = layerCount;
}
Layer::~Layer() {

}
Layer::Layer() {
	Init();
}
Layer::Layer(unsigned int x, unsigned int y)
{
	Init();
	SetImage(std::make_shared<Image>(x, y));
}
Layer::Layer(ImagePtr i) {
	Init();
	SetImage(i);//in case processing needs to happen, funnel all to SetImage
}
Layer::Layer(std::string path)
{
	Init();
	SetImage(std::make_shared<Image>(path));
}
void Layer::SetImage(ImagePtr i) {
	image = i;
}