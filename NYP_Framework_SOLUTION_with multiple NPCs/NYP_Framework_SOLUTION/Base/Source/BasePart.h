#ifndef BASE_PART_H
#define BASE_PART_H

#include "GenericEntity.h"

//base part class for all the mech parts entity
class BasePart : public GenericEntity {
public:
	BasePart(Mesh* mesh) : GenericEntity(mesh) {}
	virtual ~BasePart() {}
private:
};

#endif