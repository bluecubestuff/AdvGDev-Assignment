#include "Torso.h"

#include "MeshBuilder.h"

Torso::Torso()
	:BasePart(MeshBuilder::GetInstance()->GetMesh("torso"))
{
	hp = 100;
	size = 10;
	scale.Set(size, size, size);
	m_bCollider = false;
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
