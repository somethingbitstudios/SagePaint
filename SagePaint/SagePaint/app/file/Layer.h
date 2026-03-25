#pragma once
#include "Image.h"
#include <GL/glew.h>
enum BLEND_Type
{
	BLEND_Normal,
	BLEND_Add,
	BLEND_Multiply,
	BLEND_Darken,
	BLEND_Lighten
};

class Layer {
public:
	static unsigned int layerCount;
	ImagePtr image;
	std::string name;
	bool visible;
	float opacity;
	BLEND_Type blend;

	GLuint textureId;
	void Init();
	Layer();
	Layer(ImagePtr i);
	~Layer();

	void SetImage(ImagePtr i);
};

typedef std::shared_ptr<Layer> LayerPtr;