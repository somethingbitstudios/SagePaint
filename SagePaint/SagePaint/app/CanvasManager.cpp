#include "CanvasManager.h"
#include "shortcuts.h"
unsigned char CanvasManager::transparent[4] = { 0,0,0,0 };

CanvasObjectPtr CanvasManager::obj = nullptr;

float CanvasManager::zoom = 1.f;
bool CanvasManager::zoomType = false;
float CanvasManager::zoomRateAdditiveMacro = 0.5f;
float CanvasManager::zoomRateAdditiveMicro = 0.1f;
float CanvasManager::zoomRateMultiplicative = MY_SQRT_2;

float CanvasManager::scalingNonIntUseLinearThreshold = 2;

float CanvasManager::color[4] = { 0.0f,0.0f,0.0f,1.0f };
float CanvasManager::backgroundColor[4] = { 0.1f,0.05f,0.1f,1 };
glm::ivec2 CanvasManager::relativeCursorPos = { 0,0 };
glm::ivec2 CanvasManager::lastRelativeCursorPos = { 0,0 };


void CanvasManager::Init() {
	zoom = 1.f;
	obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);
	obj->pos.x = Screen_width / 2;//obj->image->width/2 * zoom;
	obj->pos.y = Screen_height / 2;//obj->image->height/2 * zoom;
	

}

void CanvasManager::Clear() {
	for (int i = 0; i < obj->layers->size(); i++) {
		obj->model->Discard(i);
	}
	obj->selectedLayer = -1;
	obj->model->selected_layer = -1;
	obj->layers->clear();
	if (obj->resX < 1)obj->resX = 640;
	if (obj->resY < 1)obj->resY = 400;

}
void CanvasManager::ResChange(unsigned int rX, unsigned int rY )
{
	obj->ResChange(rX,rY);
}
CanvasObjectPtr CanvasManager::GetCanvas() { return CanvasManager::obj; }

void CanvasManager::UpdateRelativeCursorPos() {
	double x = InputManager::GetCursorX();
	double y = InputManager::GetCursorY();


	
	int rel_x = ((x - obj->pos.x) / zoom + obj->resX / 2.0f);
	int rel_y = ((y - obj->pos.y) / zoom + obj->resY / 2.0f);
	
	lastRelativeCursorPos = relativeCursorPos;
	relativeCursorPos = { rel_x,rel_y };
}




void CanvasManager::Drag() {
	double dx = InputManager::GetCursorXDelta();
	double dy = InputManager:: GetCursorYDelta();
	
	obj->pos.x += ((float)dx);
	obj->pos.y += ((float)dy);
}

void CanvasManager::ZoomOut() {
	if(zoomType){
		if (zoom > 1.f) {
			DLOG("macro out")
			if (zoom <= zoomRateAdditiveMacro)return;
			glm::vec2 mouse = InputManager::GetCursorPos();
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
			glm::vec2 mouse = InputManager::GetCursorPos();
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
		glm::vec2 mouse = InputManager::GetCursorPos();
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
		
		glm::vec2 mouse = InputManager::GetCursorPos();
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
		glm::vec2 mouse = InputManager::GetCursorPos();
		glm::vec2 diff = mouse - (glm::vec2)obj->pos;
		diff *= (-1.f + zoomRateMultiplicative);
		zoom *= zoomRateMultiplicative;

		obj->SetZoom(zoom, scalingNonIntUseLinearThreshold);

		obj->pos.x -= diff.x;
		obj->pos.y -= diff.y;
	}
}


