#pragma once
#include "debug.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_NONE
#include <GLFW\glfw3.h>

#include "linmath.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../Common.h"
#include "./Model.h"
#include "../../file/Image.h"
#include "../../file/Layer.h"
//a test class for separating rendering into an object
class CanvasModel : public Model {
public:
	void Draw(glm::mat4 m, glm::mat4 p) override;
	CanvasModel();
	~CanvasModel();
	void SetImage(ImagePtr i);//deprecated

	void SetLayerVector(std::shared_ptr<std::vector<LayerPtr>> v);
	void InitLayer(int index);

	void Discard(int index);
	void SendLayerToGpu(int index);
	void SendLayersToGpu();
	/*
	void SwapLayerUp(int index);
	void SwapLayerDown(int index);
	*/
	void SetZoom(float zoom, float forceNearestThreshold); //this will update the filtering 
	void Changed()override;
	void Changed(unsigned int i);

	ImagePtr image;//deprecate?
	std::shared_ptr<std::vector<LayerPtr>> layers;
	unsigned int selected_layer;
};

typedef std::shared_ptr<CanvasModel> CanvasModelPtr;