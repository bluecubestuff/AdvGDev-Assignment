#include "Chassis.h"

#include "SceneGraph\SceneGraph.h"
#include "SceneGraph\UpdateTransformation.h"

#include "EntityManager.h"

#include "Mech.h"
#include "Torso.h"
#include "Leg.h"

Chassis::Chassis(Mech* parent)
{
	//link chassis to mech
	this->parent = parent;
	//create all the parts of the mech
	//torso
	torso = new Torso();
	CSceneNode* torsoNodetest = CSceneGraph::GetInstance()->AddNode(torso);
	leg = new Leg();
	legNode = torsoNodetest->AddChild(leg);
}

Chassis::~Chassis()
{
}

bool Chassis::GetMovability()
{
	if (leg->GetHP() <= 0)
		return false;
	return true;
}

void Chassis::Update(double dt)
{
	torso->SetPosition(parent->position);
	leg->SetPosition(Vector3(torso->GetPosition().x, torso->GetPosition().y - 5, torso->GetPosition().z));
}
