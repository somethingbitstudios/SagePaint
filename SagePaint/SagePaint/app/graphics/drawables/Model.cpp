#include "Model.h"
#include "../../file/Image.h"

Model::~Model() {
	DLOG("MODEL del")
}
Model::Model() {
	DLOG("MODEL make")
	}
void Model::Draw(glm::mat4 m, glm::mat4 p) {
	//DLOG("MODEL draw")
}
void Model::Changed() {
	//DLOG("MODEL changed")
}
