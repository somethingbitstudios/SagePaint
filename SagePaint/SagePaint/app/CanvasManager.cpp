#include "CanvasManager.h"

InputManagerPtr CanvasManager::inputManager = nullptr;

void CanvasManager::Draw() {
	DLOG("DRAW")
	double x = inputManager->GetCursorX();
	double y = inputManager->GetCursorY();
	DLOG("X: "<<x)
	DLOG("Y: "<<y)
	
}