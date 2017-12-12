#ifndef BASE_PART_H
#define BASE_PART_H

#include "GenericEntity.h"

//base part class for all the mech parts entity
class BasePart : public GenericEntity {
public:
	BasePart(Mesh* mesh) : GenericEntity(mesh) {}
	virtual ~BasePart() {}

	inline int GetHP() { return hp; }
	inline float GetSize() { return size;}
	inline void SetHP(int _hp) { hp = _hp; }
	inline void SetSize(int _size) { size = _size; }

protected:
	int hp;
	float size;
};

#endif