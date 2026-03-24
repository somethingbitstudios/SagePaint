#pragma once
#include "debug.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>
#include "./drawables/Model.h"  
enum GO_Type {
	DEFAULT,
	CANVAS
};

class GameObject {
public:
	glm::vec3 pos;
	glm::vec3 scale;
	float rotation;
	ModelPtr model;

	GameObject();
	virtual ~GameObject();

	virtual void Draw();
	virtual int GetType() const { return DEFAULT; }
};
typedef std::shared_ptr<GameObject> GameObjectPtr;