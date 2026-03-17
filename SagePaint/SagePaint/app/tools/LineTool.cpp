#include "LineTool.h"
#include "../CanvasManager.h"
#include <glm/ext/vector_int2.hpp>
glm::ivec2 LineTool::downPos = { 0,0 };
void LineTool::LineStart() {
	downPos = CanvasManager::GetRelativeCursorPos();
	DLOG("Down pos")
	DLOG(downPos.x)
	DLOG(downPos.y)
}

void LineTool::LineEnd() {
	//LineRender to the actual canvas
}
void LineTool::LinePreview() {
	//LineRender to preview canvas
}

void LineTool::LineRender() {

}
