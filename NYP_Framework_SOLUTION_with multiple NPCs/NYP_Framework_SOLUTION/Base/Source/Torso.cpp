#include "Torso.h"

#include "MeshBuilder.h"

Torso::Torso()
	:BasePart(MeshBuilder::GetInstance()->GetMesh("torso"))
{
	type = P_TORSO;
	
}
