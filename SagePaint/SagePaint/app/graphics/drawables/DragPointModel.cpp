#include "DragPointModel.h"
#include "../ShaderManager.h"

static std::string vertex_shader_text;
static std::string fragment_shader_text;


//static GLuint program;
static GLuint vertex_array;
static GLuint vertex_buffer;
//static GLuint mvp_location;


const GLuint POS_LOCATION = 0;
const GLuint UV_LOCATION = 1;
const GLuint MVP_LOCATION = 0;

static GLuint scale_location;
static float scale_width;
static float scale_inverse_aspect_ratio;
//static GLuint time_location;
static GLuint index_buffer;
static GLuint uv_buffer;
//static std::vector<GLuint> textures;
static GLuint texture;
static ImagePtr image;


static ShaderProgramPtr shader_normal;

void DragPointModel::Draw(glm::mat4 m, glm::mat4 p) {
	Model::Draw(m, p);

	

	glUseProgram(shader_normal->id);


	GLint texLoc = glGetUniformLocation(shader_normal->id, "tex");


	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 mvp_screen = p * m;
	glUniformMatrix4fv(MVP_LOCATION, 1, GL_FALSE, (const GLfloat*)&mvp_screen);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(texLoc, 0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

DragPointModel::DragPointModel() :Model() {
	image = std::make_shared<Image>("./icons/dragpoint24.png");

	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &index_buffer);
	glGenBuffers(1, &uv_buffer);

	shader_normal = ShaderManager::Get("STANDARD");




	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(POS_LOCATION);
	glVertexAttribPointer(POS_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect_uv), rect_uv, GL_STATIC_DRAW);
	glEnableVertexAttribArray(UV_LOCATION);
	glVertexAttribPointer(UV_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);

	glBindVertexArray(0);



	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image->texture);
	
	//glGenerateMipmap(GL_TEXTURE_2D);//WARN: needed?


}

DragPointModel::~DragPointModel() {
	glDeleteVertexArrays(1, &vertex_array);
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &index_buffer);
	glDeleteBuffers(1, &uv_buffer);
	glDeleteTextures(1, &texture);
}