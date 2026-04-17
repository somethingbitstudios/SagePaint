#include "CanvasModel.h"
#include "../../CanvasManager.h"
#include "../Shader.h"
#include "../ShaderManager.h"



static std::string vertex_shader_text;
static std::string fragment_shader_text;

static unsigned int instance_count = 0;

//static GLuint program;
static GLuint vertex_array;
static GLuint vertex_buffer;
//static GLuint mvp_location;

static GLint texLoc;

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

static GLuint fbo;
static GLuint compositeTexture;

static ShaderProgramPtr shader_composite_normal;
static ShaderProgramPtr shader_final;

void CanvasModel::Changed() {
	SendLayersToGpu();
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
	if (compositeTexture != 0) {
		glDeleteTextures(1, &compositeTexture);
		compositeTexture = 0; 
	}

	if (fbo != 0) {
		glDeleteFramebuffers(1, &fbo);
		fbo = 0; 
	}
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
	//DrawFbo();
}
void CanvasModel::SendLayerToGpuNoFboResized(int index) {
	ImagePtr i = (*layers)[index]->image;
	glBindTexture(GL_TEXTURE_2D, (*layers)[index]->textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		i->width, i->height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, i->texture);

	//SetZoomCurrentTexture();
}
void CanvasModel::SendLayersToGpu() {
	for (int i = 0; i < layers->size(); i++) {
		SendLayerToGpuNoFbo(i);
	}
	DrawFbo();
}
void CanvasModel::SendResizedToGpu() {
	for (int i = 0; i < layers->size(); i++) {
		SendLayerToGpuNoFboResized(i);
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
		//glDeleteProgram(shader_composite_normal->id);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &index_buffer);
		glDeleteBuffers(1, &uv_buffer);
		glDeleteTextures(1, &texture);

	}
}
void InitShader(ShaderProgramPtr prog) {

}

CanvasModel::CanvasModel() :Model() {
	selected_layer = -1;
	if (instance_count++==0) {

		glGenBuffers(1, &vertex_buffer);
		glGenBuffers(1, &index_buffer);
		glGenBuffers(1, &uv_buffer);

 		shader_composite_normal = ShaderManager::Get("CANVAS_COMPOSITE_NORMAL");
	
		shader_final = ShaderManager::Get("CANVAS_FINAL");


		scale_location = glGetUniformLocation(shader_final->id, "texSize");


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


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		
	}

}
ImagePtr CanvasModel::Export() {
	
	if (compositeImage == nullptr || compositeImage->width != resX || compositeImage->height != resY) {
		compositeImage = std::make_shared<Image>(resX, resY);

	}
	GLsizei bufSize = resX * resY * 4;
	glGetTextureImage(
		compositeTexture,        
		0,                      
		GL_RGBA,                
		GL_UNSIGNED_BYTE,        
		bufSize,                
		compositeImage->texture 
	);

	return compositeImage;
}
void CanvasModel::DrawFbo() {

	//fbo render
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, resX, resY);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);




	glUseProgram(shader_composite_normal->id);

	glEnable(GL_BLEND);

	

	glActiveTexture(GL_TEXTURE0);
	texLoc = glGetUniformLocation(shader_composite_normal->id, "tex");
	GLuint opacityLoc = glGetUniformLocation(shader_composite_normal->id, "opacity");

	glUniform1i(texLoc, 0);
	glUniform2f(scale_location, scale_width, scale_inverse_aspect_ratio);

	glm::mat4 mvp_fbo = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 1.0f));

	glUniformMatrix4fv(MVP_LOCATION, 1, GL_FALSE, (const GLfloat*)&mvp_fbo);

	glBindVertexArray(vertex_array);
	
	auto draw = [&](LayerPtr l) {
		switch (l->blend) {//this is easier for now
		case BLEND_Normal:
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;

		case BLEND_Lighten:
			glBlendEquation(GL_MAX);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			break;

		case BLEND_Darken://broken for now
			glBlendEquation(GL_MIN);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break; 

		case BLEND_Add:
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;

		case BLEND_Multiply:
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		glBindTexture(GL_TEXTURE_2D, l->textureId);
		glUniform1f(opacityLoc, l->opacity);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		};

	for (size_t i = 1; i < (*layers).size(); i++)
	{
		if ((*layers)[i]->visible) {
			draw((*layers)[i]);	
		}
	}

	if ((*layers).size()>0 && (*layers)[0]->visible) {
		if ((*layers)[0]->visible) {
			draw((*layers)[0]);
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



	glUseProgram(shader_final->id);

	
	glm::mat4 mvp_screen = p * m;
	glUniformMatrix4fv(MVP_LOCATION, 1, GL_FALSE, (const GLfloat*)&mvp_screen);
	glUniform2f(scale_location, (float)resX,resY / (float)resX);

	glBindTexture(GL_TEXTURE_2D, compositeTexture);

	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}