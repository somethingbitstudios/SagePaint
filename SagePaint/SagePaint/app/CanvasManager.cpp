#include "CanvasManager.h"
#include "shortcuts.h"
InputManagerPtr CanvasManager::inputManager = nullptr;
CanvasObjectPtr CanvasManager::obj = nullptr;

float CanvasManager::zoom = 1.f;
bool CanvasManager::zoomType = false;
float CanvasManager::zoomRateAdditiveMacro = 0.5f;
float CanvasManager::zoomRateAdditiveMicro = 0.1f;
float CanvasManager::zoomRateMultiplicative = MY_SQRT_2;

float CanvasManager::scalingNonIntUseLinearThreshold = 2;

void CanvasManager::Init() {
	obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);
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
	if(zoomType){
		if (zoom > 1.f) {
			DLOG("macro out")
			if (zoom <= zoomRateAdditiveMacro)return;
			glm::vec2 mouse = inputManager->GetCursorPos();
			glm::vec2 diff = mouse - (glm::vec2)obj->pos;
			diff *= zoomRateAdditiveMacro / zoom;
			zoom -= zoomRateAdditiveMacro;

			obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);

			obj->pos.x += diff.x;
			obj->pos.y += diff.y;
		}
		else {

			DLOG("micro out")
			if (zoom <= zoomRateAdditiveMicro)return;
			glm::vec2 mouse = inputManager->GetCursorPos();
			glm::vec2 diff = mouse - (glm::vec2)obj->pos;
			diff *= zoomRateAdditiveMicro / zoom;
			zoom -= zoomRateAdditiveMicro;

			obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);

			obj->pos.x += diff.x;
			obj->pos.y += diff.y;
		}
		
	}
	else {
		if (zoom <= 0.05f)return;
		glm::vec2 mouse = inputManager->GetCursorPos();
		glm::vec2 diff = mouse - (glm::vec2)obj->pos;
		diff *= (1.0f - 1.0f / zoomRateMultiplicative);
		
		zoom /= zoomRateMultiplicative;

		obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);

		obj->pos.x += diff.x;
		obj->pos.y += diff.y;

	}

	}
void CanvasManager::ZoomIn() {
	if (zoom > 100)return; //fixed number for now
	if (zoomType) {
		
		glm::vec2 mouse = inputManager->GetCursorPos();
		glm::vec2 diff = mouse - (glm::vec2)obj->pos;
		if (zoom >= 1.0f) {

			DLOG("macro in")
			diff *= zoomRateAdditiveMacro / zoom;
			zoom += zoomRateAdditiveMacro;
		}
		else {

			DLOG("micro in")
			diff *= zoomRateAdditiveMicro / zoom;
			zoom += zoomRateAdditiveMicro;

		}
		
		obj->SetZoom(zoom,scalingNonIntUseLinearThreshold);

		obj->pos.x -= diff.x;
		obj->pos.y -= diff.y;
	}
	else {
		glm::vec2 mouse = inputManager->GetCursorPos();
		glm::vec2 diff = mouse - (glm::vec2)obj->pos;
		diff *= (-1.f + zoomRateMultiplicative);
		zoom *= zoomRateMultiplicative;

		obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);

		obj->pos.x -= diff.x;
		obj->pos.y -= diff.y;
	}
}