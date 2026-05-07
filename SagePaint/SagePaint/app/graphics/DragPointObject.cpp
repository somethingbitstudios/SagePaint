#pragma once
#include "DragPointObject.h"
#include "drawables/DragPointModel.h"
ModelPtr DragPointObject::singleModel = nullptr;
static int instanceNum = 0;
void DragPointObject::Draw() {
	glm::mat4 m, p;
	m = glm::mat4(1.0f);

	m = glm::translate(m, pos);


	m = glm::rotate(m, -rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, scale);

	p = glm::ortho(0.0f, Screen_width, Screen_height, 0.f, -1.0f, 1.0f);
	singleModel->Draw(m, p);
}

DragPointObject::DragPointObject() {
	pos = { -1000000,-1000000,0 };//WARN:very offscreen might not always be hidden
	scale = {40,40,1};
	if (instanceNum++ == 0) {
		singleModel = std::make_shared<DragPointModel>();
	}

}
DragPointObject::~DragPointObject() {
	instanceNum--;
	if (instanceNum == 0) {
		//delete?
	}
}