#pragma once
#include "debug.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../Common.h"
#include "./Model.h"
#include "../../file/Image.h"
#include "../../file/Layer.h"

//a test class for separating rendering into an object
class DragPointModel : public Model {
public:
	void Draw(glm::mat4 m, glm::mat4 p) override;
	DragPointModel();
	~DragPointModel();
};

typedef std::shared_ptr<DragPointModel> CanvasModelPtr;