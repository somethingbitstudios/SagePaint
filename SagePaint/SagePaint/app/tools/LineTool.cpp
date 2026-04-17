#include "LineTool.h"
#include "../CanvasManager.h"
#include "../file/Layer.h"
#include <glm/ext/vector_int2.hpp>
#include <algorithm>
#include "../ProjectManager.h"
#include <imgui.h>
#include <sstream>
glm::ivec2 LineTool::downPos = { 0,0 };
glm::ivec2 lastUpPos = { 0,0 };

void LineTool::LineStart() {
	downPos = CanvasManager::GetRelativeCursorPos();
	DLOG("Down pos")
	DLOG(downPos.x)
	DLOG(downPos.y)
}

void LineTool::LineEnd() {
	//LineRender to the actual canvas
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();

	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert

	unsigned char* color;
	if (CanvasManager::erase) {
		color = CanvasManager::transparent;
		//TODO:override paint on top mode to always be replace color mode!
	}
	else {
		color = CanvasManager::color;
	}
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!

	LineRender(image->texture, image->width, image->height, downPos.x, downPos.y, lastUpPos.x, lastUpPos.y, strokeSize,CanvasManager::transparent);

	image = (*CanvasManager::obj->layers)[CanvasManager::obj->selectedLayer]->image;//WARN:hardcoded!
	LineRender(image->texture, image->width, image->height, downPos.x, downPos.y, upPos.x, upPos.y, strokeSize, color);

	CanvasManager::obj->Changed(CanvasManager::obj->selectedLayer);
	CanvasManager::obj->Changed(0);
	ProjectManager::Dirty();
}
void LineTool::LinePreview() {
	if (CanvasManager::obj->selectedLayer < 0)return;//TODO: no layer alert

/*	float* color_float = CanvasManager::colorFloat;
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
	
	glm::ivec2 upPos = CanvasManager::GetRelativeCursorPos();
	ImagePtr image = (*CanvasManager::obj->layers)[0]->image;//WARN:hardcoded!
	LineRender(image->texture, image->width, image->height, downPos.x, downPos.y, lastUpPos.x, lastUpPos.y, strokeSize, CanvasManager::transparent);
	LineRender(image->texture, image->width, image->height, downPos.x, downPos.y, upPos.x, upPos.y, strokeSize, color);

	
	CanvasManager::obj->Changed(0);
	lastUpPos = upPos;
}

void LineTool::LineRender(unsigned char* texture,int tex_w,int tex_h, int x0, int y0, int x1, int y1,float size, unsigned char color[4]) {
	// Treat size as diameter. Ensure a minimum radius so size 1 still draws a single pixel.
	float radius = std::max(0.5f, size / 2.0f);
	int r_ceil = static_cast<int>(std::ceil(radius));
	float r_sq = radius * radius;

	// Helper lambda to draw a filled circle (the brush tip) at a given point
	auto drawBrush = [&](int cx, int cy) {
		// Calculate the bounding box for the brush, clamped strictly to texture bounds
		int start_x = std::max(0, cx - r_ceil);
		int end_x = std::min(tex_w - 1, cx + r_ceil);
		int start_y = std::max(0, cy - r_ceil);
		int end_y = std::min(tex_h - 1, cy + r_ceil);

		// Fill pixels within the circle's radius
		for (int py = start_y; py <= end_y; py++) {
			for (int px = start_x; px <= end_x; px++) {
				float dx_px = static_cast<float>(px - cx);
				float dy_py = static_cast<float>(py - cy);

				// If the distance squared is within the radius squared, draw the pixel
				if ((dx_px * dx_px + dy_py * dy_py) <= r_sq) {
					int index = (px + py * tex_w) * 4;
					texture[index] = color[0];
					texture[index + 1] = color[1];
					texture[index + 2] = color[2];
					texture[index + 3] = color[3];
				}
			}
		}
		};

	// Standard DDA line calculation
	int dx = x1 - x0;
	int dy = y1 - y0;

	// The number of steps is based on the longest axis
	int steps = std::max(std::abs(dx), std::abs(dy));

	// Handle single-point dot (adx == 0 && ady == 0)
	if (steps == 0) {
		drawBrush(x0, y0);
		return;
	}

	// Calculate how much to move per step
	float x_inc = static_cast<float>(dx) / steps;
	float y_inc = static_cast<float>(dy) / steps;

	float curr_x = x0;
	float curr_y = y0;

	// Step along the line and stamp the brush at each point
	for (int i = 0; i <= steps; i++) {
		drawBrush(static_cast<int>(std::round(curr_x)), static_cast<int>(std::round(curr_y)));
		curr_x += x_inc;
		curr_y += y_inc;
	}

	/*
	float dx = x0 - x1;
	int adx = abs(dx);
	float dy = y0 - y1;
	int ady = abs(dy);

	if (ady == 0 && adx == 0) {
		if (x0 >= 0 && x0 < tex_w && y0 >= 0 && y0 < tex_h) {
			unsigned char* img = &texture[(x0 + y0 * tex_w) * 4];
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
		}
		
		return;
	}
	float dy_divide_dx = dy / dx;
	float dx_divide_dy = dx / dy;
	int y_s;
	int x_s;
	int offset=0;
	if (ady > adx) {
		if (dy >= 0) {
			y_s = y1;
			x_s = x1;
		}
		else {
			y_s = y0;
			x_s = x0;
		}
		
		if (y_s >= tex_h)return;//TODO: replace this out of bounds check
		if (y_s + ady < 0)return;
		ady = std::min(ady, tex_h - y_s - 1);//clamp to bounds, this will not work for width of line > 1
		if (y_s < 0) {
			offset = -y_s;

		}
		for (int i = offset; i <= ady; i++) {
			int x = round(x_s + (i * dx_divide_dy));
			if (x<0 || x>=tex_w)continue;//TODO: replace this out of bounds check
			unsigned char* img = &texture[(x + (y_s + i) * tex_w) * 4];
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
		}
	} 
	else {
		
		if (dx >= 0) {
			y_s = y1;
			x_s = x1;
		}
		else {
			y_s = y0;
			x_s = x0;
		}


		if (x_s >= tex_w )return;//TODO: replace this out of bounds check
		if (x_s + adx < 0)return;
		adx = std::min(adx, tex_w - x_s - 1);//clamp to bounds, this will not work for width of line > 1
		if (x_s < 0) {
			offset = -x_s;

		}
		for (int i = offset; i <= adx; i++) {
			int y = round(y_s + (i * dy_divide_dx));
			if (y<0 || y>=tex_h)continue;
			unsigned char* img = &texture[(x_s + i + (y * tex_w)) * 4];
			img[0] = color[0];
			img[1] = color[1];
			img[2] = color[2];
			img[3] = color[3];
		}
	}
	*/
}

float LineTool::strokeSize = 1;
LineMode LineTool::mode = LINE_NORMAL;
void LineTool::ShowUI() {
	ImGui::Separator();
	ImGui::Text("Line Settings:");
	if (ImGui::InputFloat("size", &strokeSize, 1, 2, "%.1f")) {
		strokeSize = std::max(0.5f, strokeSize);
	}
	const char* pencil_modes[] = { "Normal", "Rectangular" };
	static int temp_mode = 0;
	if (ImGui::Combo("Mode", &(temp_mode), pencil_modes, IM_ARRAYSIZE(pencil_modes))) {

		mode = (LineMode)temp_mode;
	}

}
std::string LineTool::ConfigString()
{
	std::stringstream ss;

	ss << R"(	{
		"strokeSize": )" << strokeSize << R"(,
		"mode": )" << static_cast<int>(mode) << R"(
	})";

	return ss.str();
}
