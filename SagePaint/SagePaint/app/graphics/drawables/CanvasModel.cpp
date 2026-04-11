#include "CanvasModel.h"
#include "../../CanvasManager.h"



static std::string vertex_shader_text;
static std::string fragment_shader_text;

static unsigned int instance_count = 0;

static GLuint program;
static GLuint vertex_array;
static GLuint vertex_buffer;
static GLuint mvp_location;
static GLint texLoc;

static GLuint scale_location;
static float scale_width;
static float scale_inverse_aspect_ratio;
//static GLuint time_location;
static GLuint index_buffer;
static GLuint uv_buffer;
//static std::vector<GLuint> textures;
static GLuint texture;

static GLuint fbo;
static GLuint compositeTexture;

void CanvasModel::Changed() {
	SendLayerToGpu(selected_layer);
}
void CanvasModel::Changed(unsigned int i) {
	SendLayerToGpu(i);
}
/*
void CanvasModel::SwapLayerUp(int index) {
	//SetImage(image);
	GLuint temp = (*layers)[index+1]->textureId;
	(*layers)[index + 1]->textureId = (*layers)[index]->textureId;
	(*layers)[index]->textureId = temp;
	//SendLayerToGpu(selected_layer);
}
void CanvasModel::SwapLayerDown(int index) {
	//SetImage(image);
	GLuint temp = (*layers)[index - 1]->textureId;
	(*layers)[index - 1]->textureId = (*layers)[index]->textureId;
	(*layers)[index]->textureId = temp;
	//SendLayerToGpu(index);
}
*/
void CanvasModel::SetZoom(float zoom, float forceNearestThreshold) {
	//DLOG(fmod(zoom,1.f))

	glBindTexture(GL_TEXTURE_2D, compositeTexture);
	if (
		zoom >= forceNearestThreshold ||
		fmod(zoom,1.0f)<0.001f //integer scaling
		
		) {//TODO:fix
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//TODO: GL_LINEAR or better filtering
		//prevents texture bleeding to its other side
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void CanvasModel::SetLayerVector(std::shared_ptr<std::vector<LayerPtr>> v) {
	layers = v;
	for (int i = 0; i < layers->size(); i++) {
		InitLayer(i);
	}
	
}

void CanvasModel::ResChange(unsigned int rX, unsigned int rY) {
	if (resX != rX || resY != rY) {
		resX = rX; resY = rY;
		DeleteFBO();
		CreateFBO();
	}
	
}
void CanvasModel::DeleteFBO() {

}
void CanvasModel::CreateFBO() {
	
	
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &compositeTexture);
	glBindTexture(GL_TEXTURE_2D, compositeTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resX, resY, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, compositeTexture, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		IDLOG("ERROR: fbo error")
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	DrawFbo();
}
void CanvasModel::InitLayer(int index){

	
	glGenTextures(1, &(*layers)[index]->textureId);
	glBindTexture(GL_TEXTURE_2D, (*layers)[index]->textureId);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_LINEAR_MIPMAP_LINEAR if gpu scaling before the fbo
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//send to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*layers)[index]->image->width, (*layers)[index]->image->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (*layers)[index]->image->texture);
	
	//glGenerateMipmap(GL_TEXTURE_2D); only if gpu scaling before fbo

	//TODO: make this not like that this is bad because it means the grid depends on last added layer
	scale_inverse_aspect_ratio = (*layers)[index]->image->height / (float)(*layers)[index]->image->width;
	scale_width = (*layers)[index]->image->width;
	//SendLayerToGpu(index);
}
void CanvasModel::Discard(int index) {
	if ((*layers)[index]->textureId != 0) {
		glDeleteTextures(1, &(*layers)[index]->textureId);

		(*layers)[index]->textureId = 0;//needed?
	}
}
void CanvasModel::SendLayerToGpu(int index) {
	ImagePtr i = (*layers)[index]->image;
	glBindTexture(GL_TEXTURE_2D, (*layers)[index]->textureId);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
		i->width, i->height,
		GL_RGBA, GL_UNSIGNED_BYTE, i->texture);
	
	//glGenerateMipmap(GL_TEXTURE_2D);
	DrawFbo();
}
void CanvasModel::SendLayerToGpuNoFbo(int index) {
	ImagePtr i = (*layers)[index]->image;
	glBindTexture(GL_TEXTURE_2D, (*layers)[index]->textureId);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
		i->width, i->height,
		GL_RGBA, GL_UNSIGNED_BYTE, i->texture);

	//glGenerateMipmap(GL_TEXTURE_2D);
	DrawFbo();
}
void CanvasModel::SendLayersToGpu() {
	for (int i = 0; i < layers->size(); i++) {
		SendLayerToGpuNoFbo(i);
	}
	DrawFbo();
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

	//change background grid
	scale_inverse_aspect_ratio = image->height / (float)image->width;
	scale_width = image->width;
	
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
	selected_layer = -1;
	if (instance_count++==0) {

		glGenBuffers(1, &vertex_buffer);
		glGenBuffers(1, &index_buffer);
		glGenBuffers(1, &uv_buffer);

		vertex_shader_text = FileManager::LoadTextFile("./shaders/canvas/shader.vert");
		const char* srcVert = vertex_shader_text.c_str();
		//DLOG("scr vert:")
		//DLOG(srcVert)
		const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &srcVert, NULL);
		glCompileShader(vertex_shader);
		 
		fragment_shader_text = FileManager::LoadTextFile("./shaders/canvas/shader.frag");
		const char* srcFrag = fragment_shader_text.c_str();
		 
		const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &srcFrag, NULL);
		glCompileShader(fragment_shader);

		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		mvp_location = glGetUniformLocation(program, "MVP");
		scale_location = glGetUniformLocation(program, "texSize");
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


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		/*glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(Vertex), (void*)offsetof(Vertex, col));
			*/
	}

}
void CanvasModel::DrawFbo() {

	//fbo render
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, resX, resY);

	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	texLoc = glGetUniformLocation(program, "tex");
	GLuint opacityLoc = glGetUniformLocation(program, "opacity");

	glUniform1i(texLoc, 0);
	glUniform2f(scale_location, scale_width, scale_inverse_aspect_ratio);

	glm::mat4 mvp_fbo = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));

	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp_fbo);

	glBindVertexArray(vertex_array);

	for (size_t i = 1; i < (*layers).size(); i++)
	{
		if ((*layers)[i]->visible) {
			glBindTexture(GL_TEXTURE_2D, (*layers)[i]->textureId);
			glUniform1f(opacityLoc, (*layers)[i]->opacity);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	glBindTexture(GL_TEXTURE_2D, compositeTexture);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Screen_width, Screen_height);
}
void CanvasModel::Draw(glm::mat4 m, glm::mat4 p) {
	//color changing
	//vertices[0].col = { abs(sin((float)glfwGetTime())),abs(cos((float)glfwGetTime())),0 };
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Model::Draw(m, p);



	//1

	GLuint opacityLoc = glGetUniformLocation(program, "opacity");
	glm::mat4 mvp_screen = p * m;
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp_screen);

	glBindTexture(GL_TEXTURE_2D, compositeTexture);

	glUniform1f(opacityLoc, 1.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}