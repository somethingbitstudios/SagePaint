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

#include "./graphics/Common.h"

//#include "graphics/TriObject.h"
#include "graphics/CanvasObject.h"

#include "input/InputManager.h"

#ifdef WINDOWS_BUILD
#include <Windows.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif
#include "CanvasManager.h"
#include "shortcuts.h"
#include <thread>

void MainApp();
void InputThreadFunction(GLFWwindow* window);