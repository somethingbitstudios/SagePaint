#include "MainApp.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>;

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>


#include "./ui/Canvas.h"
#include "ui/Triangle.h"


typedef struct Vertex
{
	vec2 pos;
	vec3 col;
} Vertex;

static const Vertex vertices[3] =
{
	{ { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
	{ {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
	{ {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void MainApp() {
	DLOG("\n----------------------------------<[DEBUG_MODE]>----------------------------------\n");
	DLOG("version: 0.03");


	

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "Sage Paint v0.01", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(1);

	
	
	Triangle triangle = Triangle();
	

	while (true)
	{

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		const float ratio = width / (float)height;

		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		triangle.scale.x = 1+0.5*sin((float)glfwGetTime());
		triangle.scale.y = 1 + 0.5 * cos((float)glfwGetTime());
		triangle.pos.x = sin((float)glfwGetTime());
		triangle.rotation = 0.1f*(float)glfwGetTime();

		triangle.Draw(ratio);


		
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwWindowShouldClose(window)) {
			DLOG("[DEBUG] Exiting application...");
			//should it end?
			glfwDestroyWindow(window);

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
	}

	
}
