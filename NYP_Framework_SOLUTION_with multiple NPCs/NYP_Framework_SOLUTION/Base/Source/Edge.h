#ifndef EDGE_H
#define EDGE_H

class Mesh;

class Node;
struct Edge {
	bool active;
	unsigned ID;
	Node* src;
	Node* dst;
	float cost;
	Mesh* mesh;

	void Set(Node* _src, Node* _dst);

	Edge();
	~Edge();
};

#endif