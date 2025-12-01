#include "TriObject.h"

TriObject::TriObject():GameObject() {

	model = std::make_shared<TriModel>();
}