#include "GameObject.h"

class DragPointObject : public GameObject {
public:
	void Draw() override;
	int GetType() const override { return GO_DRAGPOINT; }
	DragPointObject();
	~DragPointObject();
	static ModelPtr singleModel;
};
typedef std::shared_ptr<DragPointObject> DragPointObjectPtr;