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
"in vec2 vTex;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    TexCoord=vTex;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
//"uniform float time;\n"
"out vec4 fragment;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D tex;\n"
"void main()\n"
"{\n"
"    fragment = texture(tex,TexCoord);\n"
"}\n";

static unsigned int instance_count = 0;

static GLuint program;
static GLuint vertex_array;
static GLuint vertex_buffer;
static GLuint mvp_location;
//static GLuint time_location;
static GLuint index_buffer;
static GLuint uv_buffer;
static GLuint texture;


void CanvasModel::Changed() {
	SetImage(image);
}


void CanvasModel::SetImage(ImagePtr i) {
	int oldw=0,oldh=0;

	if (image != NULL) {
		int oldw = image->width;
		int oldh = image->height;

	}

	image = i;
	glBindTexture(GL_TEXTURE_2D, texture);
	if (oldw == image->width && oldh && image->height) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
			image->width, image->height,
			GL_RGBA, GL_UNSIGNED_BYTE, image->texture);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image->texture);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
}

CanvasModel::~CanvasModel() {
	instance_count--;

	if (instance_count == 0) {
		glDeleteVertexArrays(1, &vertex_array);
		glDeleteProgram(program);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &index_buffer);
		glDeleteBuffers(1, &uv_buffer);
		glDeleteTextures(1, &texture);

	}
}
CanvasModel::CanvasModel() :Model() {
	instance_count++;
	if (program == 0) {
		glGenBuffers(1, &vertex_buffer);
		glGenBuffers(1, &index_buffer);
		glGenBuffers(1, &uv_buffer);

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
		const GLint uvpos_location = glGetAttribLocation(program, "vTex");
		//const GLint vcol_location = glGetAttribLocation(program, "vCol");

		glGenVertexArrays(1, &vertex_array);
		glBindVertexArray(vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(GLfloat) * 3, (void*)0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(rect_uv), rect_uv, GL_STATIC_DRAW);

		glEnableVertexAttribArray(uvpos_location);
		glVertexAttribPointer(uvpos_location, 2, GL_FLOAT, GL_FALSE,
			sizeof(GLfloat) * 2, (void*)0);



		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



		

		/*glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, col));
			*/




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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//send uniform to shader
	GLint texLoc = glGetUniformLocation(program, "tex");
	glUniform1i(texLoc, 0);


	//consider using 'Uniform buffer objects' if there are too many uniforms
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);

	//float time_ms = (float)(glfwGetTime());
	//glUniform1f(time_location, time_ms);

	glBindVertexArray(vertex_array);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}