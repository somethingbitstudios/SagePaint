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
static std::vector<GLuint> textures;
static GLuint texture;

void CanvasModel::Changed() {
	//SetImage(image);
	SendLayerToGpu(selected_layer);
}
void CanvasModel::SetZoom(float zoom, float forceNearestThreshold) {
	//DLOG(fmod(zoom,1.f))
	if (
		zoom >= forceNearestThreshold ||
		fmod(zoom,1.0f)<0.001f //integer scaling
		
		) {//TODO:fix
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR
		//prevents texture bleeding to its other side
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void CanvasModel::SetLayerVector(std::shared_ptr<std::vector<ImagePtr>> v) {
	layers = v;
	for (int i = 0; i < layers->size(); i++) {
		InitLayer();
	}
}
void CanvasModel::InitLayer(){

	textures.push_back((GLuint)0);
	int index = textures.size() - 1;


	glGenTextures(1, &textures[index]);
	glBindTexture(GL_TEXTURE_2D, textures[index]);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//send to gpu
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (*layers)[index]->width, (*layers)[index]->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (*layers)[index]->texture);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	//TODO: make this not like that this is bad because it means the grid depends on last added layer
	scale_inverse_aspect_ratio = (*layers)[index]->height / (float)(*layers)[index]->width;
	scale_width = (*layers)[index]->width;
}

void CanvasModel::SendLayerToGpu(int index) {
	ImagePtr i = (*layers)[index];
	glBindTexture(GL_TEXTURE_2D, textures[index]);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
		(*layers)[index]->width, (*layers)[index]->height,
		GL_RGBA, GL_UNSIGNED_BYTE, (*layers)[index]->texture);
	
	glGenerateMipmap(GL_TEXTURE_2D);
}
void CanvasModel::SendLayersToGpu() {
	for (int i = 0; i < layers->size(); i++) {
		SendLayerToGpu(i);
	}
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
	
	if (instance_count++==0) {
		glGenBuffers(1, &vertex_buffer);
		glGenBuffers(1, &index_buffer);
		glGenBuffers(1, &uv_buffer);

		vertex_shader_text = FileManager::LoadTextFile("./shaders/canvas/shader.vert");
		const char* srcVert = vertex_shader_text.c_str();
		DLOG("scr vert:")
		DLOG(srcVert)
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
void CanvasModel::Draw(glm::mat4 m, glm::mat4 p) {
	//color changing
	//vertices[0].col = { abs(sin((float)glfwGetTime())),abs(cos((float)glfwGetTime())),0 };
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Model::Draw(m, p);//NEEDED?

	glm::mat4 mvp;




	mvp = p * m;

	glUseProgram(program);

	//i have "layers" (usage: layers[0]->texture gives bitmap, ->width etc)
	//i have textures (std::vector<GLuint>) that layers are bound to
	//draw all of the layers on top of each other here

	//glDisable(GL_DEPTH_TEST);


	glEnable(GL_BLEND);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);   
	//glBindTexture(GL_TEXTURE_2D, texture);


	texLoc = glGetUniformLocation(program, "tex");

	//send uniform to shader
	glUniform1i(texLoc, 0);

	glUniform2f(scale_location, scale_width, scale_inverse_aspect_ratio);


	//GLint texSizeLoc = glGetUniformLocation(program, "texSize");
	//glUniform2f(texSizeLoc, (float)image->width, (float)image->height);
	//GLint screenSizeLoc = glGetUniformLocation(program, "screenSize");
	//glUniform2f(screenSizeLoc, (float)Screen_width, (float)Screen_height);


	//consider using 'Uniform buffer objects'(?idk, meh) if there are too many uniforms
	



	
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)&mvp);

	//float time_ms = (float)(glfwGetTime());
	//glUniform1f(time_location, time_ms);

	glBindVertexArray(vertex_array);
	for (size_t i = 0; i < (*layers).size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}