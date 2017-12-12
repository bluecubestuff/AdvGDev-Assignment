#ifndef CHASSIS_H
#define CHASSIS_H

#include "GenericEntity.h"
#include "SceneGraph\SceneNode.h"

//scenenode for the body of the mech, shld not have a mesh
class Chassis : public CSceneNode {
private:
	//pos of the mech
	Chassis(Vector3 pos);
public:
	virtual ~Chassis(void);

	virtual void Update(double dt);
	virtual void Render();
};

namespace Create {

}

#endif