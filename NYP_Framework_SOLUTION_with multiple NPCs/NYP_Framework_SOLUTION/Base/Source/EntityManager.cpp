#include "EntityManager.h"
#include "EntityBase.h"
#include "Collider/Collider.h"
#include "Projectile/Laser.h"
#include "SceneGraph\SceneGraph.h"
#include "PlayerInfo\PlayerInfo.h"
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

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Update();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Update();

	// Check for Collision amongst entities with collider properties
	CheckCollisionInPartition();

	//remove obj from collisionList that are done
	it = collisionList.begin();
	while (it != collisionList.end()) {
		if ((*it)->IsDone()) {
			it = collisionList.erase(it);
		}
		else
			++it;
	}

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{	
			// Remove the node
			CSceneGraph::GetInstance()->DeleteNode((*it));
			// Remove from partition
			CSpatialPartition::GetInstance()->Remove((*it));

			// Delete if done
			delete *it;
			it = entityList.erase(it);
		}
		else // Move on otherwise
			++it;
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

	// Render the Scene Graph
	CSceneGraph::GetInstance()->Render();

	// Render the Spatial Partition
	if (theSpatialPartition)
		theSpatialPartition->Render(CPlayerInfo::GetInstance()->GetPos());
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
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);

	// Add to the Spatial Partition
	if (theSpatialPartition && bAddToSpatialPartition)
		theSpatialPartition->Add(_newEntity);

	//if (_newEntity->HasCollider() && !bAddToSpatialPartition) //add movable stuff
	if (_newEntity->HasCollider())
		collisionList.push_back(_newEntity);
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

		// Remove from SceneNode too
		if (CSceneGraph::GetInstance()->DeleteNode(_existingEntity)==false)
		{
			cout << "EntityManager::RemoveEntity: Unable to remove this entity from Scene Graph" << endl;
		}
		else
		{
			// Add to the Spatial Partition
			if (theSpatialPartition)
				theSpatialPartition->Remove(_existingEntity);
		}

		return true;	
	}
	// Return false if not found
	return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	// Return false if not found
	return false;
}

// Set a handle to the Spatial Partition to this class
void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
{
	this->theSpatialPartition = theSpatialPartition;
}

// Constructor
EntityManager::EntityManager()
	: theSpatialPartition(NULL)
{
}

// Destructor
EntityManager::~EntityManager()
{
	// Drop the Spatial Partition instance
	CSpatialPartition::GetInstance()->DropInstance();

	// Clear out the Scene Graph
	CSceneGraph::GetInstance()->Destroy();
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{	
	// Check if this object is overlapping that object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	// Check if that object is overlapping this object
	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	// Check if this object is within that object
	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
		return true;

	// Check if that object is within this object
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
		return true;

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// if Radius of bounding sphere of ThisEntity plus Radius of bounding sphere of ThatEntity is 
	// greater than the distance squared between the 2 reference points of the 2 entities,
	// then it could mean that they are colliding with each other.
	if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) + DistanceSquaredBetween(thatMinAABB, thatMaxAABB) >
		DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0)
	{
		return true;
	}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	// Get the colliders for the 2 entities
	CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	// Get the minAABB and maxAABB for each entity
	Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	// Check for overlap
	if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	// if AABB collision check fails, then we need to check the other corners of the bounding boxes to 
	// Do more collision checks with other points on each bounding box
	Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);
	//Vector3 altThatMinAABB = Vector3(thatMinAABB.x, thatMinAABB.y, thatMaxAABB.z);
	//Vector3 altThatMaxAABB = Vector3(thatMaxAABB.x, thatMaxAABB.y, thatMinAABB.z);

	// Check for overlap
	if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
		return true;

	return false;
}

// Check where a line segment between two positions intersects a plane
bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f) 
		return false;
	if (fDst1 == fDst2) 
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

// Check two positions are within a box region
bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

// Check for intersection between a line segment and a plane
bool EntityManager::CheckLineSegmentPlane(	Vector3 line_start, Vector3 line_end, 
											Vector3 minAABB, Vector3 maxAABB,
											Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) && 
			InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}

bool EntityManager::CheckCollisionInPartition(void)
{
	for (std::list<EntityBase*>::iterator it = collisionList.begin(); it != collisionList.end(); ++it)
	{
		EntityBase* entity = (EntityBase*)*it;
		if (entity->IsDone())
			continue;

		//if (entity == nullptr)
		//	continue;

		if (entity->GetIsLaser()) //for raycasting collision check 
			CheckLaserCollision(entity);
		else if (entity->obj_type == EntityBase::GRENADE)
			CheckSphereCollision(entity);
		else //not a laser
			CheckAABBCollision(entity);
	}

	return false;
}

void EntityManager::RemoveAllEntity()
{
	collisionList.clear();

	std::list<EntityBase*>::iterator it = entityList.begin();
	while (it != entityList.end())
	{
		// Remove the node
		CSceneGraph::GetInstance()->DeleteNode((*it));
		// Remove from partition
		CSpatialPartition::GetInstance()->Remove((*it));

		// Delete if done
		delete *it;
		it = entityList.erase(it);

		if (!entityList.empty())
			++it;
	}
}

bool EntityManager::CheckLaserCollision(EntityBase* _entity)
{
	//cast it to CLaser
	CLaser* laserEntity = dynamic_cast<CLaser*>(_entity);

	//end of laser
	Vector3 endPoint = laserEntity->GetPosition() - laserEntity->GetDirection() * laserEntity->GetLength();

	//check in curr grid
	int startX = (((int)laserEntity->GetPosition().x - (-CSpatialPartition::GetInstance()->GetxSize() >> 1)) / (CSpatialPartition::GetInstance()->GetxSize() / CSpatialPartition::GetInstance()->GetxNumOfGrid()));
	int startZ = (((int)laserEntity->GetPosition().z - (-CSpatialPartition::GetInstance()->GetzSize() >> 1)) / (CSpatialPartition::GetInstance()->GetzSize() / CSpatialPartition::GetInstance()->GetzNumOfGrid()));

	int endX = (((int)endPoint.x - (-CSpatialPartition::GetInstance()->GetxSize() >> 1)) / (CSpatialPartition::GetInstance()->GetxSize() / CSpatialPartition::GetInstance()->GetxNumOfGrid()));
	int endZ = (((int)endPoint.z - (-CSpatialPartition::GetInstance()->GetzSize() >> 1)) / (CSpatialPartition::GetInstance()->GetzSize() / CSpatialPartition::GetInstance()->GetzNumOfGrid()));

	int rangeX = (int)(abs(startX - endX));
	int rangeZ = (int)(abs(startZ - endZ));

	//cout << rangeX << ":" << rangeZ << endl;
	//only one grid
	if (rangeX <= 0 && rangeZ <= 0)
	{
		vector<EntityBase*> objList = CSpatialPartition::GetInstance()->GetObjects(laserEntity->GetPosition(), 1.0f);
		for (vector<EntityBase*>::iterator it = objList.begin(); it != objList.end(); ++it)
		{
			EntityBase* entity2 = (EntityBase*)(*it);

			if (entity2->obj_type == EntityBase::PLAYER_MECH)
				continue;

			if (entity2->HasCollider())
			{
				Vector3 hitPosition = Vector3(0, 0, 0);

				// Get the minAABB and maxAABB for (*colliderThat)
				CCollider *collider = dynamic_cast<CCollider*>(entity2);
				Vector3 thatMinAABB = entity2->GetPosition() + collider->GetMinAABB();
				Vector3 thatMaxAABB = entity2->GetPosition() + collider->GetMaxAABB();

				if (CheckLineSegmentPlane(laserEntity->GetPosition(),
					laserEntity->GetPosition() - laserEntity->GetDirection() * laserEntity->GetLength(),
					thatMinAABB, thatMaxAABB,
					hitPosition) == true)
				{
					//entity2->SetIsDone(true);
					//laserEntity->SetIsDone(true);
					entity2->onHit(laserEntity);
					laserEntity->onHit(entity2);

					// Remove from Scene Graph
					//if (CSceneGraph::GetInstance()->DeleteNode(entity2) == true)
					//	cout << "*** This Entity removed ***" << endl;
					
				}
			}
		}
	}
	else //check grids that line potentially passes through
	{

	}
	return false;
}

bool EntityManager::CheckAABBCollision(EntityBase * _entity)
{
	/*
		Check in multiple grid if aabb in another grid
		e.g
		if index01 = (1,1) && index02 = (3,2)

		rangeX = 2;
		rangeY = 1;

		check grid (1,1), (2,1), (3,1), (1,2), (2,2), (3,2)

		for(int i = 0; i <= rangeY; ++i)
			for(int j = 0; j <= rangeX; ++j)
			vector<EntityBase*> objList = CSpatialPartition::GetInstance()->GetGrid(j + index01.x,i + index01.y)->GetObjects(_entity->GetPosition(), 1.0f);

	*/

	int xMinIndex, zMinIndex, xMaxIndex, zMaxIndex;
	int rangeX, rangeY;

	CCollider* collider = dynamic_cast<CCollider*>(_entity);
	//get min index
	GetGridIndex((int)(collider->GetMinAABB().x), (int)(collider->GetMinAABB().z), xMinIndex, zMinIndex);
	//get max index
	GetGridIndex((int)(collider->GetMaxAABB().x), (int)(collider->GetMaxAABB().z), xMaxIndex, zMaxIndex);

	rangeX = abs(xMaxIndex - xMinIndex);
	rangeY = abs(zMaxIndex - zMinIndex);

	if (rangeX == 0 && rangeY == 0)
	{
		//if aabb is only in this grid
		vector<EntityBase*> objList = CSpatialPartition::GetInstance()->GetObjects(_entity->GetPosition(), 1.0f);
		for (std::vector<EntityBase*>::iterator it2 = objList.begin(); it2 != objList.end(); ++it2)
		{
			EntityBase* entity2 = (EntityBase*)*it2;

			//if (entity2->obj_type == _entity->obj_type)
			//	continue;
			if (entity2 == nullptr)
				continue;
			if (_entity == entity2)
				continue;

			if (entity2->HasCollider()) //aabb to aabb(check)
			{
				if (CheckSphereCollision(_entity, entity2)) //check via sphere
				{
					if (CheckAABBCollision(_entity, entity2)) //if in sphere range check aabb 
					{
						_entity->onHit(entity2);
						entity2->onHit(_entity);

						//temp set to delete
						//_entity->SetIsDone(true);
						//entity2->SetIsDone(true);
					}
				}
			}
		}
		return false;
	}
	else //in more than 1 grid
	{
		//cout << "in more than one grid boi~\n";
		for (int i = 0; i <= rangeY; ++i) //y
		{
			for (int j = 0; j <= rangeX; ++j) //x
			{
				vector<EntityBase*> objList = CSpatialPartition::GetInstance()->GetObjects(j + xMinIndex,i + zMinIndex, 1.0f);
				for (std::vector<EntityBase*>::iterator it2 = objList.begin(); it2 != objList.end(); ++it2)
				{
					EntityBase* entity2 = (EntityBase*)*it2;

					//if (entity2->obj_type == _entity->obj_type)
					//	continue;
					if (entity2 == nullptr)
						continue;
					if (_entity == entity2)
						continue;

					if (entity2->HasCollider()) //aabb to aabb(check)
					{
						if (CheckSphereCollision(_entity, entity2)) //check via sphere
						{
							if (CheckAABBCollision(_entity, entity2)) //if in sphere range check aabb 
							{
								_entity->onHit(entity2);
								entity2->onHit(_entity);

								//temp set to delete
								//_entity->SetIsDone(true);
								//entity2->SetIsDone(true);
							}
						}
					}
				} //end of check for loop
				objList.clear();
			} //end of x for loop
		} //end of y for loop
	}

	
	return false;
}

bool EntityManager::CheckSphereCollision(EntityBase * _entity)
{
	int xMinIndex, zMinIndex, xMaxIndex, zMaxIndex;
	int rangeX, rangeY;

	CCollider* collider = dynamic_cast<CCollider*>(_entity);
	//get min index
	GetGridIndex((int)(collider->GetMinAABB().x), (int)(collider->GetMinAABB().z), xMinIndex, zMinIndex);
	//get max index
	GetGridIndex((int)(collider->GetMaxAABB().x), (int)(collider->GetMaxAABB().z), xMaxIndex, zMaxIndex);

	rangeX = abs(xMaxIndex - xMinIndex);
	rangeY = abs(zMaxIndex - zMinIndex);

	if (rangeX == 0 && rangeY == 0)
	{
		//if aabb is only in this grid
		vector<EntityBase*> objList = CSpatialPartition::GetInstance()->GetObjects(_entity->GetPosition(), 1.0f);
		for (std::vector<EntityBase*>::iterator it2 = objList.begin(); it2 != objList.end(); ++it2)
		{
			EntityBase* entity2 = (EntityBase*)*it2;

			//if (entity2->obj_type == _entity->obj_type)
			//	continue;
			if (entity2 == nullptr)
				continue;
			if (_entity == entity2)
				continue;

			if (entity2->HasCollider()) //aabb to aabb(check)
			{
				if (CheckSphereCollision(_entity, entity2)) //check via sphere
				{
					_entity->onHit(entity2);
					entity2->onHit(_entity);
				}
			}
		}
		return false;
	}
	else //in more than 1 grid
	{
		//cout << "in more than one grid boi~\n";
		for (int i = 0; i <= rangeY; ++i) //y
		{
			for (int j = 0; j <= rangeX; ++j) //x
			{
				vector<EntityBase*> objList = CSpatialPartition::GetInstance()->GetObjects(j + xMinIndex, i + zMinIndex, 1.0f);
				for (std::vector<EntityBase*>::iterator it2 = objList.begin(); it2 != objList.end(); ++it2)
				{
					EntityBase* entity2 = (EntityBase*)*it2;

					//if (entity2->obj_type == _entity->obj_type)
					//	continue;
					if (entity2 == nullptr)
						continue;
					if (_entity == entity2)
						continue;

					if (entity2->HasCollider()) //aabb to aabb(check)
					{
						if (CheckSphereCollision(_entity, entity2)) //check via sphere
						{
							_entity->onHit(entity2);
							entity2->onHit(_entity);
						}
					}
				} //end of check for loop
				objList.clear();
			} //end of x for loop
		} //end of y for loop
	}


	return false;
}

void EntityManager::GetGridIndex(int posX, int posZ, int & xIndex, int & zIndex)
{
	xIndex = (((int)posX - (-CSpatialPartition::GetInstance()->GetxSize() >> 1)) / (CSpatialPartition::GetInstance()->GetxSize() / CSpatialPartition::GetInstance()->GetxNumOfGrid()));
	zIndex = (((int)posZ - (-CSpatialPartition::GetInstance()->GetzSize() >> 1)) / (CSpatialPartition::GetInstance()->GetzSize() / CSpatialPartition::GetInstance()->GetzNumOfGrid()));
}


// Check if any Collider is colliding with another Collider
bool EntityManager::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = entityList.end();
	for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (CheckLineSegmentPlane(	thisEntity->GetPosition(), 
												thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
												thatMinAABB, thatMaxAABB,
												hitPosition) == true)
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);


						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThis)) == true)
						{
							cout << "*** This Entity removed ***" << endl;
						}
						// Remove from Scene Graph
						if (CSceneGraph::GetInstance()->DeleteNode((*colliderThat)) == true)
						{
							cout << "*** That Entity removed ***" << endl;
						}

					}
				}
			}
		}
		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = entityList.end();
			int counter = 0;
			for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
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
