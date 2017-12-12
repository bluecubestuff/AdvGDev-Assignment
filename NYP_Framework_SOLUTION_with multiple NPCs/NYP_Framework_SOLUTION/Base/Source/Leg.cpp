#include "Leg.h"

#include "MeshBuilder.h"

Leg::Leg()
	:BasePart(MeshBuilder::GetInstance()->GetMesh("leg"))
{
	MeshBuilder::GetInstance()->GetMesh("leg");
	hp = 100;
	size = 100;
}

Leg::~Leg()
{
}
