#include "Chassis.h"

#include "Torso.h"
#include "Arm.h"
#include "Leg.h"
#include "Head.h"

Chassis::Chassis(Vector3 pos)
{
	//creation of all the parts
	//torso
	Torso* torso = new Torso();
	CSceneNode* torsoNode = AddChild(torso);
	//arm
	
	//leg
	
	//head

}

Chassis::~Chassis(void)
{
}

void Chassis::Update(double dt)
{
}

void Chassis::Render()
{
}
