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

#include "input/InputManager.h"

#ifdef WINDOWS_BUILD
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

InputManager inputManager;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
CanvasObjectPtr go;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
		inputManager.Input(key, action, mods);
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, GLFW_TRUE);
	

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//DLOG("[INPUT] Mouse: " << button << " Action: " << action)
		inputManager.Input(button, action, mods);
	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	//	DLOG("what")
	//}
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//DLOG("[INPUT] Mouse position: " << xpos << "x " << ypos << "y")
		

}

void MainApp() {
	IDLOG("----------------------------------<[DEBUG_MODE]>----------------------------------")
		IDLOG("IMPORTANT IDLOG MESSAGES ENABLED")
		DLOG("GRANULAR DLOG MESSAGES ENABLED")
		IDLOG("version: 0.06\n")



	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		IDLOG("GLFW failed to init!")
		exit(EXIT_FAILURE);
	}
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow((int)Screen_width, (int)Screen_height, "Sage Paint Alpha",NULL, NULL);
	if (!window)
	{
		IDLOG("Window failed to init!")
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(0);//when ==1, mouse movement caused small stutters





	//'start()'
	go = std::make_shared<CanvasObject>();
	
	ImagePtr images[] = {
		std::make_shared<Image>("C:\\temp\\test.png"),
		std::make_shared<Image>("C:\\temp\\test1.png"),
		std::make_shared<Image>("C:\\temp\\test2.png"),
		std::make_shared<Image>("C:\\temp\\test3.png"),
		std::make_shared<Image>("C:\\temp\\test4.png")
	};
	int i = 0;
	float timestamp = 0;
	static GLsync gpuFence = nullptr;

	bool runApp = true;
	while (runApp)
	{


		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		Screen_width = (float)width;
		Screen_height = (float)height;
		Screen_ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);


		glfwPollEvents();//input
		inputManager.ProcessHeld();
		//'update' like portion
		


		//go->scale.x = 1+0.5*sin((float)glfwGetTime());
		//go->scale.y = 1 + 0.5 * cos((float)glfwGetTime());
		//go->pos.x = sin((float)glfwGetTime());
		//go->rotation = 0.1f*(float)glfwGetTime();
		
		double t0 = glfwGetTime();

		
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		//DLOG(xpos << "x")
		//DLOG(ypos << "y")
		go->pos.x = ((float)xpos)-Screen_width/2;
		go->pos.y = -((float)ypos)+Screen_height/2;
		if ((float)glfwGetTime() - timestamp > 5.0f) {
			timestamp = (float)glfwGetTime();
			i++;
			i %= 5;
			go->LoadImageSync(images[i]);
			go->scale.x = (float)images[i]->width;
			go->scale.y = (float)images[i]->height;

		}
		
		go->rotation = (float)glfwGetTime()/10.0f;

		
		//render
		go->Draw();


		//printf("Frame time: %.3f ms\n", (glfwGetTime() - t0) * 1000.0);
		


		
		glfwSwapBuffers(window);

		if (glfwWindowShouldClose(window)) {
			IDLOG("Exiting application...")

				//should it end(?) logic goes here
				//glfwSetWindowShouldClose(window, GLFW_FALSE); //use when cancelling to save modified file being opened etc.

				//it should:
			runApp = false;
			}
	}
	//destruction
	
	glfwDestroyWindow(window);

	glfwTerminate();
	//exit(EXIT_SUCCESS); //prevents destructors of classes in mainApp from running 


	
}
