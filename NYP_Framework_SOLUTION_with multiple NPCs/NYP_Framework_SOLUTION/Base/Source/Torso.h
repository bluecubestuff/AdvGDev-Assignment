#ifndef TORSO_H
#define TORSO_H

#include "BasePart.h"

class Torso : public BasePart{
public:
	Torso();
	virtual ~Torso();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render() {}

private:

};

#endif