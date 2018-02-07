#ifndef NODE_H
#define NODE_H

#include <vector>

class Edge;
class Mesh;
class Vector3;
struct Node {
	bool active;
	unsigned ID;
	float x, y, size;
	std::vector<Edge*> Edges;

	void Set(float _x, float _y, float _size);

	Node();
	Node(float _x, float _y, float _size);
	~Node();
};

#endif