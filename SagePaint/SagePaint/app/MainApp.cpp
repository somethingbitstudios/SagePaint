#include "MainApp.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <glm/gtc/type_ptr.hpp>

#include "./graphics/Common.h"

#include "graphics/TriObject.h"
#include "graphics/CanvasObject.h"



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
	DLOG("\n----------------------------------<[DEBUG_MODE]>----------------------------------\n")
	DLOG("version: 0.04")


	

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		DLOG("GLFW failed to init!")
		exit(EXIT_FAILURE);
	}
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(Screen_width, Screen_height, "Sage Paint v0.01", NULL, NULL);
	if (!window)
	{
		DLOG("Window failed to init!")
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(0);//when ==1, mouse movement caused small stutters

	
	

	GameObjectPtr go = std::make_shared<CanvasObject>();

	while (true)
	{
		

		//'update' like portion
		//go->scale.x = 1+0.5*sin((float)glfwGetTime());
		//go->scale.y = 1 + 0.5 * cos((float)glfwGetTime());
		go->pos.x = sin((float)glfwGetTime());
		go->rotation = 0.1f*(float)glfwGetTime();


		//Basic gl things
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		Screen_width = width;
		Screen_height = height;
		Screen_ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		//render
		go->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwWindowShouldClose(window)) {
			DLOG("Exiting application...")
				//should it end(?) logic goes here
				//glfwSetWindowShouldClose(window, GLFW_FALSE); //use when cancelling to save modified file being opened etc.

			go.~shared_ptr();
			glfwDestroyWindow(window);

			glfwTerminate();
			exit(EXIT_SUCCESS);
		}
	}

	
}
