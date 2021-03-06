#include "Torso.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "LuaInterface.h"

Torso::Torso()
	:BasePart(MeshBuilder::GetInstance()->GetMesh("torso"))
{
	hp = CLuaInterface::GetInstance()->getIntValue("torsoHP");;
	size = 10;
	scale.Set(size, size, size);

	//add da aabb
	m_bCollider = true;
	SetAABB(scale * 0.5, -scale  * 0.5);
	EntityManager::GetInstance()->AddEntity(this, true);
	partType = BasePart::TORSO;
}

Torso::~Torso()
{
}

void Torso::Init()
{
}

void Torso::Update(double dt)
{
}

//void Torso::Render()
//{
//}
