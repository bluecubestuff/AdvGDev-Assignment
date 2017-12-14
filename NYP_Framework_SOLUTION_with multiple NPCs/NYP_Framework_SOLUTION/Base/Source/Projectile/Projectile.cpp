#include "Projectile.h"

#include "MeshBuilder.h"
#include "../EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../SpatialPartition/SpatialPartition.h"
#include "../SceneGraph/SceneGraph.h"
#include "../BasePart.h"

CProjectile::CProjectile(void)
	: modelMesh(NULL)
	, m_bStatus(false)
	, theDirection(0, 0, 0)
	, m_fLifetime(-1.0f)
	, m_fSpeed(10.0f)
	, theSource(NULL)
{
}

CProjectile::CProjectile(Mesh* _modelMesh)
	: modelMesh(_modelMesh)
	, m_bStatus(false)
	, theDirection(0, 0, 0)
	, m_fLifetime(-1)
	, m_fSpeed(10.0f)
	, theSource(NULL)
{
}

CProjectile::~CProjectile(void)
{
	modelMesh = NULL;
	theSource = NULL;
}

// Activate the projectile. true == active, false == inactive
void CProjectile::SetStatus(const bool m_bStatus)
{
	if (m_bStatus == false)
		m_fLifetime = -1;
	this->m_bStatus = m_bStatus;
}

// get status of the projectile. true == active, false == inactive
bool CProjectile::GetStatus(void) const
{
	return m_bStatus;
}

// Set the position and direction of the projectile
void CProjectile::Set(Vector3 theNewPosition, Vector3 theNewDirection, const float m_fLifetime, const float m_fSpeed)
{
	position = theNewPosition;
	theDirection = theNewDirection;
	this->m_fLifetime = m_fLifetime;
	this->m_fSpeed = m_fSpeed;
}

// Get the direction of the projectile
Vector3 CProjectile::GetDirection(void)
{
	return theDirection;
}

// Get the lifetime of the projectile
float CProjectile::GetLifetime(void) const
{
	return m_fLifetime;
}

// Get the speed of the projectile
float CProjectile::GetSpeed(void) const
{
	return m_fSpeed;
}

// Set the direction of the projectile
void CProjectile::SetDirection(Vector3 theNewDirection)
{
	theDirection = theNewDirection;
}

// Set the lifetime of the projectile
void CProjectile::SetLifetime(const float m_fLifetime)
{
	this->m_fLifetime = m_fLifetime;
}

// Set the speed of the projectile
void CProjectile::SetSpeed(const float m_fSpeed)
{
	this->m_fSpeed = m_fSpeed;
}

// Set the source of the projectile
void CProjectile::SetSource(CPlayerInfo* _source)
{
	theSource = _source;
}

// Get the source of the projectile
CPlayerInfo* CProjectile::GetSource(void) const
{
	return theSource;
}

// Update the status of this projectile
void CProjectile::Update(double dt)
{
	if (m_bStatus == false)
		return;

	// Update TimeLife of projectile. Set to inactive if too long
	m_fLifetime -= (float)dt;
	if (m_fLifetime < 0.0f)
	{
		SetStatus(false);
		SetIsDone(true);	// This method is to inform the EntityManager that it should remove this instance
		return;
	}

	// Update Position
	position.Set(	position.x + (float)(theDirection.x * dt * m_fSpeed),
					position.y + (float)(theDirection.y * dt * m_fSpeed),
					position.z + (float)(theDirection.z * dt * m_fSpeed));

	SetAABB(Vector3(scale.x * 0.5, scale.y * 0.5, scale.z * 0.5),
		Vector3(-scale.x * 0.5, -scale.x * 0.5, -scale.z * 0.5));
}


// Render this projectile
void CProjectile::Render(void)
{
	if (m_bStatus == false)
		return;

	if (m_fLifetime < 0.0f)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	//modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}

void CProjectile::onHit(EntityBase * other)
{
	if (other->obj_type == GENERIC)
	{
		other->SetIsDone(true);
		isDone = true;
	}

	if (theSource)
	{
		if (other->obj_type == PLAYER_MECH)
			return;

		if (other->obj_type == ENEMY_MECH)
		{
			BasePart* part = dynamic_cast<BasePart*>(other);
			part->SetHP(part->GetHP() - 20);
			isDone = true;

			if (part->GetHP() <= 0)
			{
				if (part->partType == BasePart::LEG)
					other->SetIsDone(true);
			}
			//other->SetIsDone(true);
		}
	}
	//else
	//{
	//	if (other->obj_type == ENEMY_MECH)
	//		return;

	//	if (other->obj_type == PLAYER_MECH)
	//	{
	//		isDone = true;
	//		other->SetIsDone(true);
	//	}
	//}

}

// Create a projectile and add it into EntityManager
CProjectile* Create::Projectile(const std::string& _meshName, 
								const Vector3& _position, 
								const Vector3& _direction, 
								const float m_fLifetime, 
								const float m_fSpeed,
								CPlayerInfo* _source)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	CProjectile* result = new CProjectile(modelMesh);
	result->Set(_position, _direction, m_fLifetime, m_fSpeed);
	result->SetStatus(true);
	result->SetCollider(true);
	result->SetSource(_source);
	result->SetScale(Vector3(1, 1, 1));
	EntityManager::GetInstance()->AddEntity(result,true);

	return result;
}