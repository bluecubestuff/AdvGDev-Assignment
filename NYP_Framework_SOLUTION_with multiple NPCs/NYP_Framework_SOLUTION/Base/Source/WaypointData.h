#ifndef WAYPOINT_H
#define WAYPOINT_H

#include "SingletonTemplate.h"
#include <vector>

class Node;
class Edge;
class WaypointData : public Singleton<WaypointData>
{
public:
	void Init();

	std::vector<Node*> nodeList;
};

#endif