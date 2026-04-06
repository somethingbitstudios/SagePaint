#include "ShapeTool.h"
#include "../CanvasManager.h"
#include <math.h>
#include "LineTool.h"
glm::ivec2 ShapeTool::downPos = { 0,0 };
glm::ivec2 lastUpPos1 = { 0,0 };
unsigned int ShapeTool::pointNumber = 640;
float ShapeTool::angleOffset = 0;
bool ShapeTool::rectConstrained = false;

void ShapeTool::ShapeStart()
{
	downPos = CanvasManager::GetRelativeCursorPos();
}

void ShapeTool::ShapePreview()
{
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	ShapeRender(image->texture, image->width, image->height, downPos, lastUpPos1, CanvasManager::transparent, rectConstrained);

	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();
	float* color_float = CanvasManager::color;
	//TODO: support width
	//TODO: optimize
	unsigned char color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	if (color[3] == 0)return;

	ShapeRender(image->texture, image->width, image->height, downPos, upPos, color, rectConstrained);

	lastUpPos1 = upPos;
	CanvasManager::obj->Changed(0);

}

void ShapeTool::ShapeRender(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4],bool rectConstrained)
{
	if (rectConstrained) { //pos0+pos1 / 2 == middle 
		//make special rect simple function without angles

		glm::vec2 middlePos = glm::vec2((startPos.x + endPos.x) / 2.0f, (startPos.y + endPos.y) / 2.0f);
		glm::vec2 initPoint = glm::vec2(-(abs(startPos.y - endPos.y) / 2.0f)*sin(angleOffset), (abs(startPos.y - endPos.y) / 2.0f)*cos(angleOffset));
		glm::vec2 lastPoint = initPoint;
		for (int i = 0; i <= pointNumber;i++) {
			float angle = (i*2 * M_PI) / pointNumber;
			float cos1 = cos(angle); float sin1 = sin(angle);
			glm::vec2 newPoint = {initPoint.x*cos1-initPoint.y*sin1,initPoint.y*cos1+initPoint.x*sin1};
			LineTool::LineRender(texture, tex_w, tex_h, round(middlePos.x+lastPoint.x), round(middlePos.y+lastPoint.y), round(middlePos.x + newPoint.x), round(middlePos.y + newPoint.y), color);
			lastPoint = newPoint;
		}


	}
	else { //pos0 == middle, pos1 == edge

		glm::vec2 middlePos = startPos;
		glm::vec2 initPoint = endPos - startPos;
		glm::vec2 lastPoint = initPoint;
		for (int i = 0; i <= pointNumber; i++) {
			float angle = (i * 2 * M_PI) / pointNumber;
			float cos1 = cos(angle); float sin1 = sin(angle);
			glm::vec2 newPoint = { initPoint.x * cos1 - initPoint.y * sin1,initPoint.y * cos1 + initPoint.x * sin1 };
			LineTool::LineRender(texture, tex_w, tex_h, round(middlePos.x + lastPoint.x), round(middlePos.y + lastPoint.y), round(middlePos.x + newPoint.x), round(middlePos.y + newPoint.y), color);
			lastPoint = newPoint;

		}
	}
}

void ShapeTool::ShapeEnd()
{
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();

	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	ShapeRender(image->texture, image->width, image->height, downPos, lastUpPos1, CanvasManager::transparent, rectConstrained);

	float* color_float = CanvasManager::color;
	//TODO: support width
	//TODO: optimize
	unsigned char color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	if (color[3] == 0)return;

	image = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;//WARN:hardcoded!
	ShapeRender(image->texture, image->width, image->height, downPos, upPos, color,rectConstrained);
	CanvasManager::obj->Changed();
	CanvasManager::obj->Changed(0);

}