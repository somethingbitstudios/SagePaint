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

void Image::Copy(unsigned char* src, int srcW, int srcH, int x, int y,int w,int h,int x2,int y2)
{
    if (w < 0)w = srcW;
    if (h < 0)h = srcH;

    // 1. Define bytes per pixel (Channels)
    // Adjust to 1 (Grayscale), 3 (RGB), or 4 (RGBA) based on your texture format.
    const int bpp = 4;

    // 2. Set up working variables
    int src_x = x;
    int src_y = y;
    int dst_x = x2;
    int dst_y = y2;
    int copy_w = w;
    int copy_h = h;

    // 3. Clip against left/top edges
    // If reading starts outside the left/top of the source buffer
    if (src_x < 0) { copy_w += src_x; dst_x -= src_x; src_x = 0; }
    if (src_y < 0) { copy_h += src_y; dst_y -= src_y; src_y = 0; }

    // If writing starts outside the left/top of the destination buffer
    if (dst_x < 0) { copy_w += dst_x; src_x -= dst_x; dst_x = 0; }
    if (dst_y < 0) { copy_h += dst_y; src_y -= dst_y; dst_y = 0; }

    // 4. Clip against right/bottom edges
    // Prevent reading past the source dimensions
    if (src_x + copy_w > srcW) { copy_w = srcW - src_x; }
    if (src_y + copy_h > srcH) { copy_h = srcH - src_y; }

    // Prevent writing past the destination dimensions
    if (dst_x + copy_w > this->width) { copy_w = this->width - dst_x; }
    if (dst_y + copy_h > this->height) { copy_h = this->height - dst_y; }

    // 5. Early exit if the chunk is completely off-screen or has invalid dimensions
    if (copy_w <= 0 || copy_h <= 0) {
        return;
    }

    // 6. Copy the memory row by row
    for (int row = 0; row < copy_h; ++row)
    {
        // Calculate the flat 1D array index for the source
        int current_src_y = src_y + row;
        int srcIndex = (current_src_y * srcW + src_x) * bpp;

        // Calculate the flat 1D array index for the destination
        int current_dst_y = dst_y + row;
        int dstIndex = (current_dst_y * this->width + dst_x) * bpp;

        // Copy exactly one row of the sub-region
        std::memcpy(this->texture + dstIndex, src + srcIndex, copy_w * bpp);
    }
}
void Image::CopyOverlay(unsigned char* src, int srcW, int srcH, int x, int y, int w, int h, int x2, int y2,float opacity)
{
    if (w < 0)w = srcW;
    if (h < 0)h = srcH;

    
    const int bpp = 4;

    int src_x = x;
    int src_y = y;
    int dst_x = x2;
    int dst_y = y2;
    int copy_w = w;
    int copy_h = h;

    if (src_x < 0) { copy_w += src_x; dst_x -= src_x; src_x = 0; }
    if (src_y < 0) { copy_h += src_y; dst_y -= src_y; src_y = 0; }

    if (dst_x < 0) { copy_w += dst_x; src_x -= dst_x; dst_x = 0; }
    if (dst_y < 0) { copy_h += dst_y; src_y -= dst_y; dst_y = 0; }

    if (src_x + copy_w > srcW) { copy_w = srcW - src_x; }
    if (src_y + copy_h > srcH) { copy_h = srcH - src_y; }

    if (dst_x + copy_w > this->width) { copy_w = this->width - dst_x; }
    if (dst_y + copy_h > this->height) { copy_h = this->height - dst_y; }

    if (copy_w <= 0 || copy_h <= 0) {
        return;
    }

    for (int row = 0; row < copy_h; ++row)
    {
        int current_src_y = src_y + row;
        int current_dst_y = dst_y + row;

        unsigned char* srcPtr = src + (current_src_y * srcW + src_x) * bpp;
        unsigned char* dstPtr = this->texture + (current_dst_y * this->width + dst_x) * bpp;

        for (int col = 0; col < copy_w; ++col)
        {
            float srcA = (srcPtr[3] / 255.0f) * opacity;
            float invA = 1.0f - srcA;

            dstPtr[0] = (unsigned char)(srcPtr[0] * srcA + dstPtr[0] * invA);
            dstPtr[1] = (unsigned char)(srcPtr[1] * srcA + dstPtr[1] * invA);
            dstPtr[2] = (unsigned char)(srcPtr[2] * srcA + dstPtr[2] * invA);

            dstPtr[3] = (unsigned char)(255 * (srcA + dstPtr[3] / 255.0f * invA));

            srcPtr += bpp;
            dstPtr += bpp;
        }
    }
}

void Image::ClearOverlay(unsigned char* src, int srcW, int srcH, int x, int y, int w, int h, int x2, int y2, float opacity)
{
    if (w < 0)w = srcW;
    if (h < 0)h = srcH;


    const int bpp = 4;

    int src_x = x;
    int src_y = y;
    int dst_x = x2;
    int dst_y = y2;
    int copy_w = w;
    int copy_h = h;

    if (src_x < 0) { copy_w += src_x; dst_x -= src_x; src_x = 0; }
    if (src_y < 0) { copy_h += src_y; dst_y -= src_y; src_y = 0; }

    if (dst_x < 0) { copy_w += dst_x; src_x -= dst_x; dst_x = 0; }
    if (dst_y < 0) { copy_h += dst_y; src_y -= dst_y; dst_y = 0; }

    if (src_x + copy_w > srcW) { copy_w = srcW - src_x; }
    if (src_y + copy_h > srcH) { copy_h = srcH - src_y; }

    if (dst_x + copy_w > this->width) { copy_w = this->width - dst_x; }
    if (dst_y + copy_h > this->height) { copy_h = this->height - dst_y; }

    if (copy_w <= 0 || copy_h <= 0) {
        return;
    }

    for (int row = 0; row < copy_h; ++row)
    {
        int current_src_y = src_y + row;
        int current_dst_y = dst_y + row;

        unsigned char* srcPtr = src + (current_src_y * srcW + src_x) * bpp;
        unsigned char* dstPtr = this->texture + (current_dst_y * this->width + dst_x) * bpp;

        for (int col = 0; col < copy_w; ++col)
        {
            float srcA = (srcPtr[3] / 255.0f) * opacity;
            float invA = 1.0f - srcA;

         
            dstPtr[3] = (unsigned char)((dstPtr[3] * invA));

            srcPtr += bpp;
            dstPtr += bpp;
        }
    }
}
void Image::Clear(int x, int y, int w, int h)
{
    const int bpp = 4;

   
    
    int copy_w = w;
    int copy_h = h;

    //left top bound
    if (x < 0) { copy_w += x; x = 0; }
    if (y < 0) { copy_h += y; y = 0; }
    //right bottom bound
    if (x + copy_w > this->width) { copy_w = this->width - x; }
    if (y + copy_h > this->height) { copy_h = this->height - y; }
    
    if (copy_w <= 0 || copy_h <= 0) {
        return;
    }

    for (int row = 0; row < copy_h; ++row)
    {
        int current_dst_y = y + row;
        int dstIndex = (current_dst_y * this->width + x) * bpp;
        std::fill(this->texture + dstIndex, this->texture + dstIndex + copy_w * bpp,0);
    }
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
