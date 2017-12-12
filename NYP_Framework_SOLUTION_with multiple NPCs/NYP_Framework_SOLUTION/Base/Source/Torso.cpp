#include "Torso.h"

#include "MeshBuilder.h"

Torso::Torso()
	:BasePart(MeshBuilder::GetInstance()->GetMesh("torso"))
{
	hp = 100;
	size = 100;
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

void Torso::Render()
{
}
