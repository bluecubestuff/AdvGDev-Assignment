#include "Edge.h"

#include "Node.h"
#include "MeshBuilder.h"
#include "WaypointData.h"

void Edge::Set(Node* _src, Node* _dst)
{
	src = _src;
	dst = _dst;
	Vector3 v(abs(dst->x - src->x), abs(dst->y - src->y));
	cost = v.Length();

	mesh = MeshBuilder::GetInstance()->GenerateLine(Vector3(src->x, 0, src->y), Vector3(dst->x, 0, dst->y), Color(0, 1, 0));
}

Edge::Edge()
{
	active = false;
}

Edge::~Edge()
{
	//deletes the mesh
	delete mesh;
}
