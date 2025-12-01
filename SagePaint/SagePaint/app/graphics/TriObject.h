#pragma once
#include "GameObject.h"
#include "./drawables/TriModel.h"
class TriObject: public GameObject {
public:
	TriObject();
};
typedef std::shared_ptr<TriObject> TriObjectPtr;