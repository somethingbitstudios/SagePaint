#include "GameObject.h"




GameObject::~GameObject() {
	DLOG("GOBJ Destroy")
}


GameObject::GameObject() {
	DLOG("GOBJ Init")
	pos = { 0,0,0 };
	scale = { 1,1,1 };
	rotation = 0;
}
void GameObject::Draw() {
	glm::mat4 m, p, mvp;
	m = glm::mat4(1.0f);

	m = glm::translate(m, pos);


	m = glm::rotate(m, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	m = glm::scale(m, scale);
	//game-like ortho where making window smaller scales objects to fit
	p = glm::ortho(-Screen_ratio, Screen_ratio, -1.0f, 1.0f, 1.0f, -1.0f);

	model->Draw(m, p);
}