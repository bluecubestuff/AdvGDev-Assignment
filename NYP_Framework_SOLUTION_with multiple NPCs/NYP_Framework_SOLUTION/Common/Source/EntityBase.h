#ifndef ENTITY_BASE_H
#define ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	//enum of type
	enum OBJECT_TYPE
	{
		NONE = 0,
		//PLAYER_BULLET,
		//ENEMY_BULLET,
		PLAYER_MECH,
		ENEMY_MECH,
		GENERIC,
		GRENADE,
	} obj_type;

	virtual void Update(double _dt);
	virtual void Render();
	virtual void RenderUI();

	inline void SetPosition(const Vector3& _value){ position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetScale(const Vector3& _value){ scale = _value; };
	inline Vector3 GetScale(){ return scale; };

	bool IsDone();
	void SetIsDone(const bool _value);

	// Check if this entity has a collider class parent
	virtual bool HasCollider(void) const;
	// Set the flag to indicate if this entity has a collider class parent
	virtual void SetCollider(const bool _value);
	// Set the flag, bLaser
	virtual void SetIsLaser(const bool bLaser);
	// Get the flag, bLaser
	virtual bool GetIsLaser(void) const;

	//set what happens on collision
	virtual void onHit(EntityBase* other);

	//set obj type
	virtual void setObjectType(OBJECT_TYPE _type);

protected:
	Vector3 position;
	Vector3 scale;

	bool isDone;
	bool m_bCollider;
	bool bLaser;
};

#endif // ENTITY_BASE_H