#include "CanvasModel.h"


/* this shader set uses time and position in fragment, the position might not be optimal because 2x MVP * vec4 is not needed
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
//"uniform float time;\n" //not needed if time is only needed in the fragment shader
"in vec3 vPos;\n"
"out vec4 pospos;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"	 pospos = MVP*vec4(vPos,1.0);\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"uniform float time;\n"
"out vec4 fragment;\n"
"in vec4 pospos;\n"
"void main()\n"
"{\n"
"    fragment = vec4(abs(sin(time+pospos.x)),abs(cos(time+pospos.y)),abs(cos(-time-(100.0*pospos.x))), 1.0);\n"
"}\n";
*/


static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"

"in vec3 vPos;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
//"uniform float time;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(1.0,1.0,1.0, 1.0);\n"
"}\n";

static unsigned int instance_count = 0;

static GLuint program;
static GLuint vertex_array;
static GLuint vertex_buffer;
static GLuint mvp_location;
//static GLuint time_location;
static GLuint index_buffer;

CanvasModel::~CanvasModel() {
	instance_count--;

	if (instance_count == 0) {
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &index_buffer);

	}
}
CanvasModel::CanvasModel() :Model() {
	instance_count++;
	if (program == 0) {
		glGenBuffers(1, &vertex_buffer);
		glGenBuffers(1, &index_buffer);

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
		//time_location = glGetUniformLocation(program, "time");

		const GLint vpos_location = glGetAttribLocation(program, "vPos");
		//const GLint vcol_location = glGetAttribLocation(program, "vCol");

		// ===== VAO SETUP (correct order) =====
		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

		// ---- bind VBO *after* binding VAO ----
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

		// ---- bind EBO while VAO is bound ----
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

		// ---- setup vertex attributes ----
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(GLfloat)*3, (void*)0);

		/*glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, col));
			*/
		// ===== DO NOT create another VAO here =====


		
	}

}
void CanvasModel::Draw(glm::mat4 m, glm::mat4 p) {
	//color changing
	//vertices[0].col = { abs(sin((float)glfwGetTime())),abs(cos((float)glfwGetTime())),0 };
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Model::Draw(m, p);//NEEDED?
	glm::mat4 mvp;


	mvp = p * m;
	glUseProgram(program);
	//consider using 'Uniform buffer objects' if there are too many uniforms
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);

	//float time_ms = (float)(glfwGetTime());
	//glUniform1f(time_location, time_ms);
	
	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}