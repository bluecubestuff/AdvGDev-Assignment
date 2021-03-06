#ifndef BASE_PART_H
#define BASE_PART_H

#include "GenericEntity.h"

//base part class for all the mech parts entity
class BasePart : public GenericEntity {
public:
	BasePart(Mesh* mesh) : GenericEntity(mesh) {}
	virtual ~BasePart() {}

	enum PART_TYPE
	{
		NONE = 0,
		TORSO,
		LEG,
	} partType;

	inline int GetHP() { return hp; }
	inline float GetSize() { return size;}
	inline void SetHP(int _hp) { hp = _hp; }
	inline void SetSize(int _size) { size = _size; }

	virtual void onHit(EntityBase* other) {};
protected:
	int hp;
	float size;
};

#endif