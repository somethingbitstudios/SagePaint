#include "Canvas.h"
#include "Common.h"
#include <glm/ext/matrix_float4x4.hpp>
#include <linmath.h>
void Canvas::Draw() {
	DLOG("pos:");
	DLOG(pos.x);
	DLOG(pos.y);
	DLOG("size");
	DLOG(size.x);
	DLOG(size.y);

	
	glVertexPointer(3, GL_FLOAT, 0, rect_vertices);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, rect_indices);

}
Canvas::Canvas(glm::vec2 p, glm::vec2 s){
	pos =  p;
	size = s;

}
Canvas::Canvas() {

}
Canvas::~Canvas() {

}