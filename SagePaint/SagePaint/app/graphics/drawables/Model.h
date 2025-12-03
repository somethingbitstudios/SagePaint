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
#include "../Common.h"
//a test class for separating rendering into an object
class Model {
public:
	Model();
	~Model();
	virtual void Draw(glm::mat4 m,glm::mat4 p);
	virtual void Changed();
};

typedef std::shared_ptr<Model> ModelPtr;