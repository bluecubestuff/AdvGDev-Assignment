#ifndef BASE_PART_H
#define BASE_PART_H

#include "GenericEntity.h"

class BasePart : public GenericEntity {
public:
	enum PART_TYPE {
		P_NONE,
		P_ARM,
		P_HEAD,
		P_TORSO,
		P_LEG
	}type;

	BasePart(Mesh* _modelMesh) : GenericEntity(_modelMesh) {}
	virtual ~BasePart() {}
};

#endif