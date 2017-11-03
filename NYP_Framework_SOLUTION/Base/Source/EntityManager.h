#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"
#include <list>
#include "Vector3.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(EntityBase* _newEntity);
	bool RemoveEntity(EntityBase* _existingEntity);

private:
	EntityManager();
	virtual ~EntityManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check for intersection between line segment and plane
	bool GetIntersection(const float dist1, const float dist2, Vector3 pt1, Vector3 pt2, Vector3 &Hit);
	// Check for intersection between line segment and plane
	bool CheckLineSegmentPlane(Vector3 line_start,Vector3 line_end,Vector3 minAABB,Vector3 maxAABB, Vector3 &Hit);
	// Check 2 pos are with the box region
	bool InBox(Vector3 hit, Vector3 b1, Vector3 b2, const int axis);

	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(void);

	std::list<EntityBase*> entityList;
};

#endif // ENTITY_MANAGER_H