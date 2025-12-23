#include "CanvasManager.h"

InputManagerPtr CanvasManager::inputManager = nullptr;
CanvasObjectPtr CanvasManager::obj = nullptr;
void CanvasManager::Init() {
	obj->pos.x = obj->image->width/2;
	obj->pos.y = obj->image->height /2;
}
void CanvasManager::Draw() {
	
	double x = inputManager->GetCursorX();
	double y = inputManager->GetCursorY();
	
	//	obj->pos.x = ((float)x) - Screen_width / 2;
	//obj->pos.y = -((float)y) + Screen_height / 2;
	
}


void CanvasManager::Drag() {
	double dx = inputManager->GetCursorXDelta();
	double dy = inputManager->GetCursorYDelta();
	
	obj->pos.x += ((float)dx);
	obj->pos.y += ((float)dy);
}
