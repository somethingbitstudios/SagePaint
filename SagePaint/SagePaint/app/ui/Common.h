#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int3.hpp>
#include <GL/glew.h>


GLfloat rect_vertices[] = { -1, -1, 0, // bottom left corner
					  -1,  1, 0, // top left corner
					   1,  1, 0, // top right corner
					   1, -1, 0 }; // bottom right corner

GLubyte rect_indices[] = { 0,1,2, 
					 0,2,3 };



typedef struct Vertex
{
	glm::vec2 pos;
	glm::vec3 col;
} Vertex;
