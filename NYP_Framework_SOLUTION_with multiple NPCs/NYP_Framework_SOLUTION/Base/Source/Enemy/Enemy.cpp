#include "Enemy.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "..\PlayerInfo\PlayerInfo.h"
#include "../Mech.h"
#include "../Chassis.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "..\Leg.h"
#include "..\Chassis.h"
#include "..\Mech.h"
#include "..\enemygun.h"

#include "../WaypointData.h"
#include "../Node.h"
#include "../Edge.h"

#define AGRO_DIST 10000.f

CEnemy::CEnemy()
	: GenericEntity(NULL)
	, defaultPosition(Vector3(0.0f, 0.0f, 0.0f))
	, defaultTarget(Vector3(0.0f, 0.0f, 0.0f))
	, defaultUp(Vector3(0.0f, 0.0f, 0.0f))
	, target(Vector3(0.0f, 0.0f, 0.0f))
	, up(Vector3(0.0f, 0.0f, 0.0f))
	, maxBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, minBoundary(Vector3(0.0f, 0.0f, 0.0f))
	, m_pTerrain(NULL)
	, m_iSeed(0)
{
}


CEnemy::~CEnemy()
{
}

void CEnemy::Init(void)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(10.0f, 0.0f, 0.0f);
	if (m_pTerrain)
		target = GenerateTarget();
	else
		target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	// Set speed
	m_dSpeed = 10.0;

	// Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	rotateSpeed = 45.f;
	moveDir.Set(1, 0, 0);

	gunno = new CEnemyGun();
	gunno->Init();

	// Initialise the Collider
	//this->SetCollider(true);
	//this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

void CEnemy::Init(float x, float y)
{
	// Set the default values
	defaultPosition.Set(0, 0, 10);
	defaultTarget.Set(0, 0, 0);
	defaultUp.Set(0, 1, 0);

	// Set the current values
	position.Set(x, 0.0f, y);
	if (m_pTerrain)
		target = GenerateTarget();
	else
		target.Set(10.0f, 0.0f, 450.0f);
	up.Set(0.0f, 1.0f, 0.0f);

	// Set Boundary
	maxBoundary.Set(1, 1, 1);
	minBoundary.Set(-1, -1, -1);

	rotateSpeed = 45.f;
	moveDir.Set(1, 0, 0);

	// Set speed
	m_dSpeed = 10.0;

	// Initialise the LOD meshes
	InitLOD("cube", "sphere", "cubeSG");

	gunno = new CEnemyGun();
	gunno->Init();

	// Initialise the Collider
	//this->SetCollider(true);
	//this->SetAABB(Vector3(1, 1, 1), Vector3(-1, -1, -1));

	// Add to EntityManager
	EntityManager::GetInstance()->AddEntity(this, true);
}

// Reset this player instance to default
void CEnemy::Reset(void)
{
	// Set the current values to default values
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

// Set position
void CEnemy::SetPos(const Vector3& pos)
{
	position = pos;
}

// Set target
void CEnemy::SetTarget(const Vector3& target)
{
	this->target = target;
}
// Set Up
void CEnemy::SetUp(const Vector3& up)
{
	this->up = up;
}
// Set the boundary for the player info
void CEnemy::SetBoundary(Vector3 max, Vector3 min)
{
	maxBoundary = max;
	minBoundary = min;
}
// Set the terrain for the player info
void CEnemy::SetTerrain(GroundEntity* m_pTerrain)
{
	if (m_pTerrain != NULL)
	{
		this->m_pTerrain = m_pTerrain;

		SetBoundary(this->m_pTerrain->GetMaxBoundary(), this->m_pTerrain->GetMinBoundary());
	}
}

bool CEnemy::AStar(Vector3 end)
{
	//a star open and closed list
	std::list<PathFindNode> openList;
	std::list<unsigned> closedList;
	//previous vector of nodes
	std::vector<int> previous;
	previous.resize(WaypointData::GetInstance()->nodeList.size());
	std::fill(previous.begin(), previous.end(), -1);

	//make npc move to the nearest node
	unsigned start = WaypointData::GetInstance()->GetNearestNode(position)->ID;
	unsigned endNode = WaypointData::GetInstance()->GetNearestNode(end)->ID;

	//push the first PathNode into the list
	openList.push_back(PathFindNode(start, endNode, start));

	while (!openList.empty())
	{
		Node* curr = WaypointData::GetInstance()->GetNode(openList.front().nodeID);
		closedList.push_back(openList.front().nodeID);
		openList.pop_front();

		//check if curr is end alr
		if (curr->ID == endNode)
		{
			path.clear();
			path.push_back(curr);
			while (curr->ID != start)
			{
				curr = WaypointData::GetInstance()->GetNode(previous[curr->ID]);
				path.push_back(curr);
			}
			return true;
		}


		//push in all the surronding nodes into the open list if they not in close
		for (auto it : curr->Edges)
		{
			bool visited = false;
			//check if dst has been visited
			for (auto visitedNode : closedList)
			{
				if (visitedNode == it->dst->ID)
				{
					visited = true;
					break;
				}
			}
			//if not visited
			if (!visited)
			{
				bool inserted = false;
				//the new node to be inserted
				PathFindNode newPFNode(it->dst->ID, endNode, start);
				//sort insert the nodes into open
				for (std::list<PathFindNode>::iterator openNode = openList.begin(); openNode != openList.end(); ++openNode)
				{
					PathFindNode _curr = *openNode;
					if (newPFNode.f_cost < _curr.f_cost)
					{
						//if the f_cost is lower, insert it infront
						openList.insert(openNode, newPFNode);
						inserted = true;
						break;
					}
				}
				if (!inserted)
					openList.push_back(newPFNode);

				//set the previous
				previous[newPFNode.nodeID] = curr->ID;
			}
		}
	}
	return false;
}

// Get position
Vector3 CEnemy::GetPos(void) const
{
	return position;
}

// Get target
Vector3 CEnemy::GetTarget(void) const
{
	return target;
}
// Get Up
Vector3 CEnemy::GetUp(void) const
{
	return up;
}
// Get the terrain for the player info
GroundEntity* CEnemy::GetTerrain(void)
{
	return m_pTerrain;
}

// Update
void CEnemy::Update(double dt)
{
	if (!attached->chassis->GetMovability())
		return;

	//enemy will just chase the player
	target = CPlayerInfo::GetInstance()->GetPos();
	Vector3 viewVector = (target - position);
	if (viewVector.LengthSquared() > AGRO_DIST) {
		//if the enemy leg still can move
		if (attached->chassis->GetLeg()->GetHP() > 0) {
			//rotate enemy to move towards player
			Mtx44 mtx;
			//get angle between view and move
			float dot = viewVector.x * moveDir.x + viewVector.z + moveDir.z;
			float det = viewVector.x*moveDir.z - viewVector.z*moveDir.x;
			float angle = Math::RadianToDegree(atan2(dot, det));
			//positive is right
			if (angle > -90 && angle < 90) {
				mtx.SetToRotation(rotateSpeed * dt, 0, 1, 0);
				moveDir = mtx * moveDir;
			}
			else {
				//negative is left
				mtx.SetToRotation(-rotateSpeed * dt, 0, 1, 0);
				moveDir = mtx * moveDir;
			}

			position += moveDir.Normalized() * (float)m_dSpeed * (float)dt;
			//cout << position << " - " << target << "..." << viewVector << endl;
		}
	}
	//shooto
	Vector3 shootDir = CPlayerInfo::GetInstance()->GetPos() - position;
	shootDir.Normalize();
	shootDir.x += Math::RandFloatMinMax(-0.1 , 0.1);
	shootDir.y += Math::RandFloatMinMax(-0.1, 0.1);
	shootDir.z += Math::RandFloatMinMax(-0.1, 0.1);
	gunno->Discharge(position, position + shootDir);
	gunno->Update(dt);

	// Constrain the position
	Constrain();



	//// Update the target
	//if (position.z > 400.0f)
	//	target.z = position.z * -1;
	//else if (position.z < -400.0f)
	//	target.z = position.z * -1;
}

// Constrain the position within the borders
void CEnemy::Constrain(void)
{
	// Constrain player within the boundary
	if (position.x > maxBoundary.x - 1.0f)
		position.x = maxBoundary.x - 1.0f;
	if (position.z > maxBoundary.z - 1.0f)
		position.z = maxBoundary.z - 1.0f;
	if (position.x < minBoundary.x + 1.0f)
		position.x = minBoundary.x + 1.0f;
	if (position.z < minBoundary.z + 1.0f)
		position.z = minBoundary.z + 1.0f;

	if (abs(((target.x - position.x) * (target.x - position.x) - (target.z - position.z)*(target.z - position.z))) < m_dSpeed)
	{
		target = GenerateTarget();
	}

	// if the y position is not equal to terrain height at that position, 
	// then update y position to the terrain height
	if (position.y != m_pTerrain->GetTerrainHeight(position))
		position.y = m_pTerrain->GetTerrainHeight(position);
}

// Render
void CEnemy::Render(void)
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (GetLODStatus() == true)
	{
		if (theDetailLevel != NO_DETAILS)
		{
			//cout << theDetailLevel << endl;
			RenderHelper::RenderMesh(GetLODMesh());
		}
	}
	modelStack.PopMatrix();
}

// Generate New Target
Vector3 CEnemy::GenerateTarget(void)
{
	return Vector3(	rand() % (int)((maxBoundary.x - minBoundary.x)*0.5),	
					0.0f, 
					rand() % (int)((maxBoundary.x - minBoundary.x)*0.5));
}

// Set random seed
void CEnemy::SetRandomSeed(const int m_iSeed)
{
	this->m_iSeed = m_iSeed;
}

PathFindNode::PathFindNode(unsigned _node, unsigned dstID, unsigned start)
{
	nodeID = _node;
	Node* currNode = WaypointData::GetInstance()->GetNode(_node);
	Node* dstNode = WaypointData::GetInstance()->GetNode(dstID);
	Node* startNode = WaypointData::GetInstance()->GetNode(start);

	Vector3 g(abs(currNode->x - startNode->x), abs(currNode->y - startNode->y));
	g_cost = g.Length();
	Vector3 h(abs(currNode->x - dstNode->x), abs(currNode->y - dstNode->y));
	h_cost = h.Length();
	f_cost = g_cost + h_cost;
}
