#include "PencilTool.h"
#include "../CanvasManager.h"


void PencilTool::Stroke() {
	double x = InputManager::GetCursorX();
	double y = InputManager::GetCursorY();

	//TODO: combine these canvasmanager calls maybe?
	CanvasObjectPtr obj = CanvasManager::GetCanvas();
	float zoom = CanvasManager::zoom;
	float* color = CanvasManager::color;

	int relX = ((x - obj->pos.x) / zoom + obj->image->width / 2.0f);
	int relY = ((y - obj->pos.y) / zoom + obj->image->height / 2.0f);
	//is target pixel in image? (not just mouse, the precise pixel)
	if (relX >= 0 && relX < obj->image->width && relY >= 0 && relY < obj->image->height) {
		int index = (relX + relY * obj->image->width) * 4;
		obj->image->texture[index] = color[0] * 255;
		obj->image->texture[index + 1] = color[1] * 255;
		obj->image->texture[index + 2] = color[2] * 255;
		obj->image->texture[index + 3] = color[3] * 255;
		obj->Changed();

	}

}