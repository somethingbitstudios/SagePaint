#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>;
#include <Debug.h>
#include <glm/ext/vector_float2.hpp>

class Canvas {
public:
	glm::vec2 pos;
	glm::vec2 size;
	void Draw();
	Canvas(glm::vec2 p, glm::vec2 s);
	Canvas();
	~Canvas();
};

typedef std::shared_ptr<Canvas> CanvasPtr;