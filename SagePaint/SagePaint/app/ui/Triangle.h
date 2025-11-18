#include "Debug.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>;

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

//a test class for separating rendering into an object
class Triangle {
public:
	glm::vec3 pos;
	glm::vec3 scale;
	float rotation;
	Triangle();
	void Draw(float ratio);
};