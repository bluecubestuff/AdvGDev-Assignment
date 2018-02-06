#include "WaypointData.h"

#include "Node.h"
#include "Edge.h"

void WaypointData::Init()
{
	float gridSize = 100;
	int gridCount = 10;
	for (int i = -gridCount / 2; i < gridCount / 2; ++i)
	{
		for (int j = -gridCount / 2; j < gridCount / 2; ++j)
		{
			Node* newNode = new Node();
			nodeList.push_back(newNode);
			newNode->ID = nodeList.size() - 1;
			newNode->x = i * gridSize + gridSize / 2;
			newNode->y = j * gridSize + gridSize / 2;
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
		}
		if (i + gridCount < gridCount * gridCount)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i + gridCount]);
			nodeList[i]->Edges.push_back(edgy);
		}
		if (i + 1 < gridCount * gridCount)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i + 1]);
			nodeList[i]->Edges.push_back(edgy);
		}
		if (i - 1 >= 0)
		{
			edgy = new Edge();
			edgy->Set(nodeList[i], nodeList[i - 1]);
			nodeList[i]->Edges.push_back(edgy);
		}
	}
}
