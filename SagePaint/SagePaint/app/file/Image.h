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
	void Copy(unsigned char * src,int srcW,int srcH,int x,int y,int w,int h,int x2,int y2);
	void CopyOverlay(unsigned char * src,int srcW,int srcH,int x,int y,int w,int h,int x2,int y2,float opacity);
	void ClearOverlay(unsigned char * src,int srcW,int srcH,int x,int y,int w,int h,int x2,int y2,float opacity);
	void Clear(int x, int y, int w, int h);
	unsigned char* texture;
	int width;
	int height;
	int id;//WARN: debug
private:
	void LoadImage(std::string path);

};

typedef std::shared_ptr<Image> ImagePtr;