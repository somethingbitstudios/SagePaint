#include "Icons.h"
std::vector<ImTextureID> Icons::icons;

void Icons::AddIcon(std::string path,std::vector<ImTextureID> &vec) {
	GLuint texId;
	ImagePtr image = std::make_shared<Image>(path);

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image->texture);

	vec.push_back((void*)(intptr_t)texId);
}
