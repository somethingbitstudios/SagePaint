#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

//mostly wrap for now
void Image::LoadImage(std::string path) {
	int n = 0;
	texture =  stbi_load(path.c_str(), &width, &height, &n, 4);
}
void Image::Init() {
	width = 0;
	height = 0;
	texture = NULL;
	DLOG("Image init")

}
Image::Image() {
	Image::Init();
	}

Image::Image(std::string path) {
	Image::Init();
	LoadImage(path);
}
Image::~Image() {
	stbi_image_free(texture);
	DLOG("Image end")
}
