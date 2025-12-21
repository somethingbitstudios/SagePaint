#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
int nums = 0;//WARN: debug 

//mostly wrap for now
void Image::LoadImage(std::string path) {
	texture =  FileManager::LoadImage(path, &width, &height); //WARN: replace with call to FileManager, that can change and be easily made to work async
}
void Image::Init() {
	width = 0;
	height = 0;
	texture = NULL;
	DLOG("Image init")
		DLOG(++nums)
		id = nums;
}

Image::Image() {
	//DLOG("Image empty constr.")
	Image::Init();
	}
Image::Image(int w, int h) {
	DLOG("Image init (w,h)")
		DLOG(++nums)
		id = nums;
	//DLOG("Image w.h. constr.")
	width = w;
	height = h;
	int n = 4;//number of channels
	texture = (unsigned char*)calloc(w*h*n,sizeof(unsigned char)); //for empty canvas

}
Image::Image(std::string path) {
	//DLOG("Image path constr.")
	Image::Init();
	LoadImage(path);
}
Image::~Image() {
	stbi_image_free(texture);
	DLOG("Image end")
	DLOG(id)
}
