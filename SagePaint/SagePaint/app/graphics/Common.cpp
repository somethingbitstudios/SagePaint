#include "Common.h"

GLfloat rect_vertices[] = {
    -0.5f, -0.5f, 0,
    -0.5f,  0.5f, 0,
     0.5f,  0.5f, 0,
     0.5f, -0.5f, 0
};
GLfloat rect_uv[] = {
    0, 0,
    1.0,0,
    1.0,1.0,
    0,1.0
};
GLuint rect_indices[] = {
    0, 1, 2,
    0, 2, 3
};

float Screen_ratio = 1;
float Screen_width = 640;
float Screen_height = 480;