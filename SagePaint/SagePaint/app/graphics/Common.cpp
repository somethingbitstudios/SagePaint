#include "Common.h"

GLfloat rect_vertices[] = {
    -0.5f, -0.5f, 0,
    -0.5f,  0.5f, 0,
     0.5f,  0.5f, 0,
     0.5f, -0.5f, 0
};
GLfloat rect_uv[] = {//made so that textures are 'rotated' by 90degs, because of the way rotation is done now, might be edited later
    0, 1.0,
    0,0,
    1.0,0,
    1.0,1.0
};
GLuint rect_indices[] = {
    0, 1, 2,
    0, 2, 3
};

float Screen_ratio = 1;
float Screen_width = 1280;
float Screen_height = 720;