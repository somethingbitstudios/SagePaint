#pragma once
#include "debug.h"
#include "FileManager.h"
class Image {
public:
	Image();
	Image(std::string path);
	Image(int w,int h);

	~Image();

	void Init();

	unsigned char* texture;
	int width;
	int height;
	int id;//WARN: debug
private:
	void LoadImage(std::string path);

};

typedef std::shared_ptr<Image> ImagePtr;