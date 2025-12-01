#include "TriModel.h"



static Vertex vertices[3] =
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
static unsigned int instance_count=0;

static GLuint program;
static GLuint vertex_array;
static GLuint vertex_buffer;
static GLuint mvp_location;

TriModel::~TriModel() {
	instance_count--;

	if (instance_count == 0) {
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertex_buffer);
		
	}
}
TriModel::TriModel():Model() {
	instance_count++;
	if (program == 0) {
		
		glGenBuffers(1, &vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
		glCompileShader(vertex_shader);

		const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		mvp_location = glGetUniformLocation(program, "MVP");
		const GLint vpos_location = glGetAttribLocation(program, "vPos");
		const GLint vcol_location = glGetAttribLocation(program, "vCol");


		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, col));
	}

}
void TriModel::Draw(glm::mat4 m, glm::mat4 p) {
	//color changing
	//vertices[0].col = { abs(sin((float)glfwGetTime())),abs(cos((float)glfwGetTime())),0 };
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Model::Draw(m, p);//NEEDED?
	glm::mat4 mvp;


	mvp = p * m;
	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);

	glBindVertexArray(vertex_array);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}