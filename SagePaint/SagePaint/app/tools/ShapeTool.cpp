#include "ShapeTool.h"
#include "../CanvasManager.h"
#include <math.h>
#include "LineTool.h"
#include "../ProjectManager.h"
#include <imgui.h>
#include <shortcuts.h>
#include <sstream>

glm::ivec2 ShapeTool::downPos = { 0,0 };
glm::ivec2 lastUpPos1 = { 0,0 };
int ShapeTool::pointNumber = 3;
float ShapeTool::angleOffset = 0;

void ShapeTool::ShapeStart()
{
	downPos = CanvasManager::GetRelativeCursorPos();
}

void ShapeTool::ShapePreview()
{
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	ShapeRender(image->texture, image->width, image->height, downPos, lastUpPos1, CanvasManager::transparent, mode);

	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();
	
	/*float* color_float = CanvasManager::colorFloat;
	//TODO: support width
	//TODO: optimize
	unsigned char color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	if (color[3] == 0)return;
	*/
	unsigned char* color;
	if (CanvasManager::erase) {
		color = CanvasManager::erasePreviewColor;
		//TODO:override paint on top mode to always be replace color mode!
	}
	else {
		color = CanvasManager::color;
	}

	ShapeRender(image->texture, image->width, image->height, downPos, upPos, color, mode);

	lastUpPos1 = upPos;
	CanvasManager::obj->Changed(0);

}

//split into multiple functions for performance and clarity
void ShapeTool::ShapeRender(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4],ShapeMode mode)
{
	if (mode == SHAPE_FAST) {
		//top line
		LineTool::LineRender(texture,tex_w,tex_h,startPos.x,startPos.y,endPos.x,startPos.y, strokeSize, color);
		LineTool::LineRender(texture,tex_w,tex_h,startPos.x,endPos.y,endPos.x,endPos.y, strokeSize, color);
		if (abs(startPos.y - endPos.y) > 1) {
			if (startPos.y < endPos.y) {
				LineTool::LineRender(texture, tex_w, tex_h, startPos.x, startPos.y + 1, startPos.x, endPos.y - 1, strokeSize, color);//
				LineTool::LineRender(texture, tex_w, tex_h, endPos.x, startPos.y + 1, endPos.x, endPos.y - 1, strokeSize, color);//

			}
			else {
				LineTool::LineRender(texture, tex_w, tex_h, startPos.x, startPos.y - 1, startPos.x, endPos.y + 1, strokeSize, color);//
				LineTool::LineRender(texture, tex_w, tex_h, endPos.x, startPos.y - 1, endPos.x, endPos.y + 1, strokeSize, color);//

			}
			}

	}
	else if (mode == SHAPE_INRECT) { //pos0+pos1 / 2 == middle 
		//make special rect simple function without angles

		glm::vec2 middlePos = glm::vec2((startPos.x + endPos.x) / 2.0f, (startPos.y + endPos.y) / 2.0f);
		glm::vec2 initPoint = glm::vec2(-(abs(startPos.y - endPos.y) / 2.0f)*sin(angleOffset), (abs(startPos.y - endPos.y) / 2.0f)*cos(angleOffset));
		//float cs = cos(angleOffset); float sn = sin(angleOffset);
		//initPoint = { initPoint.x * cs - initPoint.y * sn, initPoint.y * cs + initPoint.x * sn };

		glm::vec2 lastPoint = initPoint;
		for (int i = 1; i <= pointNumber;i++) {
			float angle = (i*2 * M_PI) / pointNumber;
			float cos1 = cos(angle); float sin1 = sin(angle);
			glm::vec2 newPoint = {initPoint.x*cos1-initPoint.y*sin1,initPoint.y*cos1+initPoint.x*sin1};
			LineTool::LineRender(texture, tex_w, tex_h, round(middlePos.x+lastPoint.x), round(middlePos.y+lastPoint.y), round(middlePos.x + newPoint.x), round(middlePos.y + newPoint.y), strokeSize, color);
			lastPoint = newPoint;
		}


	}
	else { //pos0 == middle, pos1 == edge

		glm::vec2 middlePos = startPos;
		glm::vec2 initPoint = endPos - startPos;
		float cs = cos(angleOffset); float sn = sin(angleOffset);
		initPoint = { initPoint.x* cs - initPoint.y * sn, initPoint.y* cs + initPoint.x * sn };
		glm::vec2 lastPoint = initPoint;
		for (int i = 1; i <= pointNumber; i++) {
			float angle = (i * 2 * M_PI) / pointNumber;
			float cos1 = cos(angle); float sin1 = sin(angle);
			glm::vec2 newPoint = { initPoint.x * cos1 - initPoint.y * sin1,initPoint.y * cos1 + initPoint.x * sin1 };
			LineTool::LineRender(texture, tex_w, tex_h, round(middlePos.x + lastPoint.x), round(middlePos.y + lastPoint.y), round(middlePos.x + newPoint.x), round(middlePos.y + newPoint.y), strokeSize, color);
			lastPoint = newPoint;

		}
	}
}

void ShapeTool::ShapeRenderFilledRect(unsigned char* texture, int tex_w, int tex_h, glm::ivec2 startPos, glm::ivec2 endPos, unsigned char color[4])
{
	if (!texture || tex_w <= 0 || tex_h <= 0) return;

	//border check
	int x_min = std::min(startPos.x, endPos.x);
	int x_max = std::max(startPos.x, endPos.x);
	int y_min = std::min(startPos.y, endPos.y);
	int y_max = std::max(startPos.y, endPos.y);
	x_min = std::max(0, x_min);
	x_max = std::min(tex_w - 1, x_max);
	y_min = std::max(0, y_min);
	y_max = std::min(tex_h - 1, y_max);

	//offscreen test
	if (x_min > x_max || y_min > y_max) {
		return;
	}

	// 4byte array => uint32
	uint32_t fill_color;
	std::memcpy(&fill_color, color, 4);

	
	int line = tex_w * 4;

	for (int y = y_min; y <= y_max; ++y) {
		unsigned char* row_byte_ptr = texture + (y * line);

		uint32_t* row_pixel_ptr = reinterpret_cast<uint32_t*>(row_byte_ptr);

		//magic
		std::fill(row_pixel_ptr + x_min, row_pixel_ptr + x_max + 1, fill_color);
	}
}

void ShapeTool::ShapeEnd()
{
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();

	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	ShapeRender(image->texture, image->width, image->height, downPos, lastUpPos1, CanvasManager::transparent, mode);

	/*float* color_float = CanvasManager::colorFloat;
	//TODO: support width
	//TODO: optimize
	unsigned char color[4] = { color_float[0] * 255,color_float[1] * 255,color_float[2] * 255,color_float[3] * 255 };//make this only happen once per color setting
	if (color[3] == 0)return;
	*/

	unsigned char* color;
	if (CanvasManager::erase) {
		color = CanvasManager::transparent;
		//TODO:override paint on top mode to always be replace color mode!
	}
	else {
		color = CanvasManager::color;
	}
	image = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;//WARN:hardcoded!
	ShapeRender(image->texture, image->width, image->height, downPos, upPos, color, mode);

	CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);
	CanvasManager::obj->Changed(0);
	ProjectManager::Dirty();

}
float ShapeTool::strokeSize = 1.0f;
ShapeMode ShapeTool::mode = SHAPE_NORMAL;

void ShapeTool::ShowUI() {
	ImGui::Separator();
	ImGui::Text("Shape Settings:");
	if (ImGui::InputFloat("line size", &strokeSize, 1, 2, " %.1f")) {
		strokeSize = std::max(0.5f, strokeSize);
	}
	if (ImGui::InputInt("point n.", &pointNumber, 1, 2)) {
		pointNumber = std::max(3, pointNumber);
	}
	static float degreeOffset = 0;
	if (ImGui::InputFloat("offset", &degreeOffset, 1, 2, "%.2f")) {
		angleOffset = degreeOffset * DEG_TO_RAD;
		DLOG(angleOffset)
		DLOG(degreeOffset)
	}
	const char* pencil_modes[] = { "Normal", "Rect. Constrained","Fast rectangle"};
	static int temp_mode = 0;
	if (ImGui::Combo("Mode", &(temp_mode), pencil_modes, IM_ARRAYSIZE(pencil_modes))) {

		mode = (ShapeMode)temp_mode;
	}
}

std::string ShapeTool::ConfigString()
{
	std::stringstream ss;

	ss << R"(	{
		"strokeSize": )" << strokeSize << R"(,
		"mode": )" << static_cast<int>(mode) << R"(
	})";

	return ss.str();
}
