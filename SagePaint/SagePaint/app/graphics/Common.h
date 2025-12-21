#ifndef COMMON_UI
#define COMMON_UI
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>

#include <glm/ext/vector_int3.hpp>
#include <GL/glew.h>


extern GLfloat rect_vertices[12]; 
extern GLuint rect_indices[6];
extern GLfloat rect_uv[8];
extern float Screen_ratio;
extern float Screen_width;
extern float Screen_height;

typedef struct Vertex
{
	glm::vec2 pos;
	glm::vec3 col;
} Vertex;
#endif 