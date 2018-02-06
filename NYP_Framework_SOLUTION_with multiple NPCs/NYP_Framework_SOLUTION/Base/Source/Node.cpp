#include "Node.h"

#include "MeshBuilder.h"

void Node::Set(float _x, float _y, float _size = 1.f)
{
	x = _x;
	y = _y;
	size = _size;
}

Node::Node()
{
	active = false;
}

Node::Node(float _x = 0.f, float _y = 0.f, float _size = 1.f)
{
	x = _x;
	y = _y;
	size = _size;
	active = false;
}

Node::~Node()
{
}
