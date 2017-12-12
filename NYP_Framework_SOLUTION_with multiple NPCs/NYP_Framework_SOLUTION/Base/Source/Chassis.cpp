#include "Chassis.h"

#include "SceneGraph\SceneGraph.h"
#include "SceneGraph\UpdateTransformation.h"

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
	torsoNode = CSceneGraph::GetInstance()->AddNode((GenericEntity*)torso);
	leftLeg = new Leg();
	rightLeg = new Leg();
	CSceneNode* leftLegNode = torsoNode->AddChild(leftLeg);
	CSceneNode* rightLegNode = torsoNode->AddChild(rightLeg);
}

Chassis::~Chassis()
{
}

bool Chassis::GetMovability()
{
	if (leftLeg->GetHP() <= 0 || rightLeg->GetHP() <= 0)
		return false;
	return true;
}

void Chassis::Update(double dt)
{
	torso->SetPosition(parent->position);
	CUpdateTransformation* mtx = new CUpdateTransformation();
	mtx->ApplyUpdate(torso->GetPosition().x, torso->GetPosition().y, torso->GetPosition().z);
	torsoNode->SetUpdateTransformation(mtx);
}
