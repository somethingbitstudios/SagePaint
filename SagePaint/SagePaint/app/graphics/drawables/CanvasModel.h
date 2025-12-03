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
#include "./Model.h"
#include "../../file/Image.h"
//a test class for separating rendering into an object
class CanvasModel : public Model {
public:
	void Draw(glm::mat4 m, glm::mat4 p) override;
	CanvasModel();
	~CanvasModel();
	void SetImage(ImagePtr i);
	void Changed()override;
	ImagePtr image;
};

typedef std::shared_ptr<CanvasModel> CanvasModelPtr;