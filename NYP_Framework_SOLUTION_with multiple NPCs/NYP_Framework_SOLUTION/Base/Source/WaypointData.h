#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "SingletonTemplate.h"
#include <vector>

class Node;
class Edge;
class Vector3;
class WaypointData : public Singleton<WaypointData>
{
public:
	void Init();
	Node* GetNode(unsigned ID);
	Node* GetNearestNode(Vector3 pos);
	Edge* GetEdge(unsigned ID);

	void GenerateFromFile();
	void DeleteWaypoints();

	std::vector<Node*> nodeList;
	std::vector<Edge*> edgeList;
};

#endif