#include "WaypointData.h"

#include "Node.h"
#include "Edge.h"
#include "Vector3.h";
#include "LuaInterface.h"

void WaypointData::Init()
{
	float gridSize = 100;
	int gridCount = 10;
	char variableName[80] = "";

	CLuaInterface::GetInstance()->saveIntValue("WAYPOINTS", gridCount * gridCount, true);

	for (int i = -gridCount / 2; i < gridCount / 2; ++i)
	{
		for (int j = -gridCount / 2; j < gridCount / 2; ++j)
		{
			Node* newNode = new Node();
			nodeList.push_back(newNode);
			newNode->ID = nodeList.size() - 1;
			newNode->x = i * gridSize + gridSize / 2;
			newNode->y = j * gridSize + gridSize / 2;

			//temp method
			sprintf(variableName, "Node_%d", newNode->ID);
			CLuaInterface::GetInstance()->saveFloatValue(variableName, newNode->x);
			sprintf(variableName, "Node_%d", newNode->ID);
			CLuaInterface::GetInstance()->saveFloatValue(variableName, newNode->y);

			newNode->active = true;
			newNode->size = 2.f;
		}
	}

	for (int i = 0; i < nodeList.size(); ++i)
	{
		Edge* edgy;
		if (i - gridCount >= 0)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i - gridCount]);
			nodeList[i]->Edges.push_back(edgy);
			edgeList.push_back(edgy);
			edgy->ID = edgeList.size() - 1;
		}
		if (i + gridCount < gridCount * gridCount)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i + gridCount]);
			nodeList[i]->Edges.push_back(edgy);
			edgeList.push_back(edgy);
			edgy->ID = edgeList.size() - 1;
		}
		if (i + 1 < gridCount * gridCount)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i + 1]);
			nodeList[i]->Edges.push_back(edgy);
			edgeList.push_back(edgy);
			edgy->ID = edgeList.size() - 1;
		}
		if (i - 1 >= 0)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i - 1]);
			nodeList[i]->Edges.push_back(edgy);
			edgeList.push_back(edgy);
			edgy->ID = edgeList.size() - 1;
		}
	}
}

Node * WaypointData::GetNode(unsigned ID)
{
	return nodeList[ID];
}

Node * WaypointData::GetNearestNode(Vector3 pos)
{
	int distance = INT_MAX;
	Node* nearestNode = nullptr;
	for (auto it : nodeList)
	{
		Vector3 v = pos - Vector3(it->x, it->y);
		if (v.Length() < distance)
		{
			nearestNode = it;
		}
	}
	return nearestNode;
}

Edge * WaypointData::GetEdge(unsigned ID)
{
	return edgeList[ID];
}
