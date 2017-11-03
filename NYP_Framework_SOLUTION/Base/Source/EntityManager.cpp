#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile\Laser.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double _dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	//check all da collisions
	CheckForCollision();

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity)
{
	entityList.push_back(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete *findIter;
		findIter = entityList.erase(findIter);
		return true;	
	}
	// Return false if not found
	return false;
}

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	//check if this obj overlap that obj
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	//get the collider for both entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//get minAABB & maxAABB for entites
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB)
		> DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()))
		return true;

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//get minAABB & maxAABB for entites
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	//cehck for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMinAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMaxAABB.z);

	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

bool EntityManager::GetIntersection(const float dist1, const float dist2, Vector3 pt1, Vector3 pt2, Vector3 & Hit)
{
	if((dist1 * dist2) >= 0.0f)
		return false;
	if (dist1 == dist2)
		return false;
	Hit = pt1 + (pt2 - pt1) * (-dist1 / (dist2 - dist1));
	return true;
}

bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end, Vector3 minAABB, Vector3 maxAABB, Vector3 & Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x,
		line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y -
			minAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z -
			minAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x -
			maxAABB.x, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y -
			maxAABB.y, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z -
			maxAABB.z, line_start, line_end, Hit) && InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

bool EntityManager::InBox(Vector3 hit, Vector3 b1, Vector3 b2, const int axis)
{
	if (axis == 1 && hit.z > b1.z && hit.z < b2.z && hit.y > b1.y && hit.y < b2.y)
		return true;
	if (axis == 2 && hit.z > b1.z && hit.z < b2.z && hit.x > b1.x && hit.x < b2.x)
		return true;
	if (axis == 3 && hit.x > b1.x && hit.x < b2.x && hit.y > b1.y && hit.y < b2.y)
		return true;

	return false;
}

// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	//check da collisions
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		if ((*colliderThis)->GetIsLaser() == true)
		{
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			//check for collision
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPos = Vector3(0, 0, 0);

					//get minAABB & maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB, hitPos))
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);
					}
				}

			}
		}
		else if ((*colliderThis)->HasCollider())
		{
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			//check collision with another 
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (CheckSphereCollision(thisEntity, thatEntity))
					{
						if (CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
						}
					}
				}
			}
		}
	}

	return false;
}
