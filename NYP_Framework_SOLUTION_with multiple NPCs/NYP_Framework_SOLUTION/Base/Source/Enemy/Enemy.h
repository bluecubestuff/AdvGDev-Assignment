#pragma once
#include "../GenericEntity.h"
#include "../GroundEntity.h"
#include <vector>

class Mesh;
class Mech;
class CEnemyGun;
class Node;

struct PathFindNode
{
	int g_cost, f_cost, h_cost;
	unsigned nodeID;

	PathFindNode(unsigned _node, unsigned dstID, unsigned start);
	~PathFindNode() {}
};

class CEnemy :
	public GenericEntity
{
protected:
	Vector3 defaultPosition, defaultTarget, defaultUp;
	Vector3 target, up;
	Vector3 maxBoundary, minBoundary;
	GroundEntity* m_pTerrain;

	double m_dSpeed;
	double m_dAcceleration;

	int m_iSeed;

	Vector3 moveDir;
	float rotateSpeed;

	CEnemyGun* gunno;
	std::vector<Node*> path;

	int playerIndexX, playerIndexZ;

public:
	CEnemy(void);
	virtual ~CEnemy();

	enum STATE
	{
		S_LOITER,
		S_CHASE,
		S_COUNT
	}state;

	Mech* attached;

	void Init(void);
	void Init(float x, float y);
	// Reset this player instance to default
	void Reset(void);

	// Set position
	void SetPos(const Vector3& pos);
	// Set target
	void SetTarget(const Vector3& target);
	// Set Up
	void SetUp(const Vector3& up);
	// Set the boundary for the player info
	void SetBoundary(Vector3 max, Vector3 min);
	// Set the terrain for the player info
	void SetTerrain(GroundEntity* m_pTerrain);

	bool AStar(Vector3 end);

	Vector3 GetMoveDir() { return moveDir; }

	// Get position
	Vector3 GetPos(void) const;
	// Get target
	Vector3 GetTarget(void) const;
	// Get Up
	Vector3 GetUp(void) const;
	// Get the terrain for the player info
	GroundEntity* GetTerrain(void);

	// Update
	void Update(double dt = 0.0333f);

	// Constrain the position within the borders
	void Constrain(void);
	// Render
	void Render(void);
	// Set random seed
	void SetRandomSeed(const int m_iSeed);
	// Generate New Target
	Vector3 GenerateTarget(void);
};
