#include "Chassis.h"

#include "SceneGraph\SceneGraph.h"
#include "SceneGraph\UpdateTransformation.h"

#include "EntityManager.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"

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

	if (parent->controlType == Mech::PLAYER)
	{
		torso->setObjectType(EntityBase::PLAYER_MECH);
		leg->setObjectType(EntityBase::PLAYER_MECH);
	}
	else if (parent->controlType == Mech::NPC)
	{
		torso->setObjectType(EntityBase::ENEMY_MECH);
		leg->setObjectType(EntityBase::ENEMY_MECH);
	}
}

Chassis::~Chassis()
{
}

bool Chassis::GetMovability()
{
	if (leg->GetHP() <= 0 && torso->GetHP() <= 0)
		return false;
	return true;
}

void Chassis::Update(double dt)
{
	torso->SetPosition(parent->position);
	leg->SetPosition(Vector3(torso->GetPosition().x, torso->GetPosition().y - 7.5, torso->GetPosition().z));

	if (torso->GetHP() <= 0)
	{
		if (torso->obj_type == EntityBase::ENEMY_MECH)
		{
			parent->attachedEntity->SetIsDone(true);
			parent->isDead = true;
		}
		if (torso->obj_type == EntityBase::PLAYER_MECH)
			leg->SetHP(0);

	}

	//cout << "TorsePos: " << torso->GetPosition() << endl;
	//cout << "LegPos: " << leg->GetPosition() << endl;
	/*MS& ms = GraphicsManager::GetInstance()->GetModelStack();
	ms.PushMatrix();
	ms.LoadIdentity();
	ms.Translate(leg->GetPosition().x, leg->GetPosition().y, leg->GetPosition().z);
	float rotate = Math::RadianToDegree(atan2(parent->legDirection.y, parent->legDirection.x));
	ms.Rotate(rotate, 0, 1, 0);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("leg"));
	ms.PopMatrix();*/
}
