#include "Leg.h"

#include "MeshBuilder.h"
#include "EntityManager.h"

Leg::Leg()
	:BasePart(MeshBuilder::GetInstance()->GetMesh("leg"))
{
	MeshBuilder::GetInstance()->GetMesh("leg");
	hp = 100;
	size = 5;
	scale.Set(size, size, size);

	//add da aabb
	m_bCollider = true;
	SetAABB(scale  * 0.5, -scale  * 0.5);
	EntityManager::GetInstance()->AddEntity(this, true);
}

Leg::~Leg()
{
}
