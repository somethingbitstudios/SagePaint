#pragma once
#include "debug.h"

class Image {
public:
	Image();
	Image(std::string path);
	~Image();

	void LoadImage(std::string path);
	void Init();
	unsigned char* texture;
	int width;
	int height;
};

typedef std::shared_ptr<Image> ImagePtr;