#include "LineTool.h"
#include "../CanvasManager.h"
#include <glm/ext/vector_int2.hpp>
#include <algorithm>
glm::ivec2 LineTool::downPos = { 0,0 };
void LineTool::LineStart() {
	downPos = CanvasManager::GetRelativeCursorPos();
	DLOG("Down pos")
	DLOG(downPos.x)
	DLOG(downPos.y)
}

void LineTool::LineEnd() {
	//LineRender to the actual canvas
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();
	LineRender(CanvasManager::obj->image->texture, CanvasManager::obj->image->width, CanvasManager::obj->image->height, downPos.x, downPos.y, upPos.x, upPos.y,1);
	CanvasManager::obj->Changed();
}
void LineTool::LinePreview() {
	/*glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();
	LineRender(CanvasManager::obj->image->texture, CanvasManager::obj->image->width, CanvasManager::obj->image->height, downPos.x, downPos.y, upPos.x, upPos.y, 1);
	CanvasManager::obj->Changed();
	*/
	//LineRender to preview canvas
}

void LineTool::LineRender(unsigned char* texture,int tex_w,int tex_h, int x0, int y0, int x1, int y1, int width ) {
	float* color_float = CanvasManager::color;
	//TODO: support width

	//TODO: optimize
	int color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting

	float dx = x0 - x1;
	int adx = abs(dx);
	float dy = y0 - y1;
	int ady = abs(dy);
	float dy_divide_dx = dy / dx;
	float dx_divide_dy = dx / dy;
	int y_s;
	int x_s;
	int offset=0;
	if (ady > adx) {
		if (dy >= 0) {
			y_s = y1;
			x_s = x1;
		}
		else {
			y_s = y0;
			x_s = x0;
		}
		
		if (y_s >= tex_h)return;//TODO: replace this out of bounds check
		if (y_s + ady < 0)return;
		ady = std::min(ady, tex_h - y_s - 1);//clamp to bounds, this will not work for width of line > 1
		if (y_s < 0) {
			offset = -y_s;

		}
		for (int i = offset; i <= ady; i++) {
			int x = round(x_s + (i * dx_divide_dy));
			if (x<0 || x>=tex_w)continue;//TODO: replace this out of bounds check
			unsigned char* img = &texture[(x + (y_s + i) * tex_w) * 4];
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
		}
	} 
	else {
		
		if (dx >= 0) {
			y_s = y1;
			x_s = x1;
		}
		else {
			y_s = y0;
			x_s = x0;
		}


		if (x_s >= tex_w )return;//TODO: replace this out of bounds check
		if (x_s + adx < 0)return;
		adx = std::min(adx, tex_w - x_s - 1);//clamp to bounds, this will not work for width of line > 1
		if (x_s < 0) {
			offset = -x_s;

		}
		for (int i = offset; i <= adx; i++) {
			int y = round(y_s + (i * dy_divide_dx));
			if (y<0 || y>=tex_h)continue;
			unsigned char* img = &texture[(x_s + i + (y * tex_w)) * 4];
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
		}
	}
	
}
