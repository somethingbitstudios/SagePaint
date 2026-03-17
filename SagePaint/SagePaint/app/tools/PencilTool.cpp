#include "PencilTool.h"
#include "../CanvasManager.h"


void PencilTool::Stroke() {
	
	glm::ivec2 rel = CanvasManager::GetRelativeCursorPos();
	CanvasObjectPtr obj = CanvasManager::GetCanvas();
	float* color_float = CanvasManager::color;

	//TODO: optimize
	int color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	int radius = 16;


	int start_x = rel.x - radius; int end_x = rel.x + radius;
	int start_y = rel.y - radius; int end_y = rel.y + radius;
	//don't draw if it's not even on the canvas
	if (end_x < 0 || end_y < 0 || start_x >= obj->image->width || start_y >= obj->image->height) {
		return;
	}
	//not sure if optimal, meh
	start_x = std::max(start_x, 0);
	start_y = std::max(start_y, 0);
	end_x = std::min(end_x, obj->image->width-1);
	end_y = std::min(end_y, obj->image->height-1);
	int difference_x = end_x - start_x;
	for (int i = start_y; i <= end_y; i++) {
		unsigned char* img = &obj->image->texture[(start_x + i * obj->image->width)*4]; //should be faster than it being in the for bellow
		for (int j = 0; j <= difference_x; j++) {
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
			img += 4;
				

			
		}
	}
	obj->Changed();
	

}
void PencilTool :: StrokeStart() {

}
void PencilTool::StrokeEnd() {

}