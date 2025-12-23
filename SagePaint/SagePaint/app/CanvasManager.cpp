#include "CanvasManager.h"

InputManagerPtr CanvasManager::inputManager = nullptr;
CanvasObjectPtr CanvasManager::obj = nullptr;

float CanvasManager::zoom = 1.f;

void CanvasManager::Init() {
	obj->SetZoom(zoom);
	obj->pos.x = obj->image->width/2 * zoom;
	obj->pos.y = obj->image->height/2 * zoom;

}
void CanvasManager::Draw() {
	
	double x = inputManager->GetCursorX();
	double y = inputManager->GetCursorY();
	
	int relX = ((x - obj->pos.x) / zoom + obj->image->width/2.0f);
	int relY = ((y - obj->pos.y) / zoom + obj->image->height / 2.0f);
	//is target pixel in image? (not just mouse, the precise pixel)
	if (relX >= 0 && relX < obj->image->width && relY >= 0 && relY < obj->image->height) {
		int index = (relX + relY * obj->image->width) * 4;
		obj->image->texture[index] = 0;
		obj->image->texture[index + 1] = 0;
		obj->image->texture[index + 2] = 0;
		obj->image->texture[index + 3] = 0;
		obj->Changed();

	}
	
}


void CanvasManager::Drag() {
	double dx = inputManager->GetCursorXDelta();
	double dy = inputManager->GetCursorYDelta();
	
	obj->pos.x += ((float)dx);
	obj->pos.y += ((float)dy);
}

void CanvasManager::ZoomOut() {
	zoom -= 0.2f;
	obj->SetZoom(zoom);
}
void CanvasManager::ZoomIn() {
	zoom += 0.2f;
	obj->SetZoom(zoom);
}