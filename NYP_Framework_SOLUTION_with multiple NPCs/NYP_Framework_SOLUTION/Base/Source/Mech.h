#ifndef MECH_H
#define MECH_H

#include "Chassis.h"
#include "Vector3.h"

class GenericEntity;

//main class for the mechs, all controls happen here
class Mech {
public:
	enum CONTROL_TYPE {
		PLAYER,
		NPC
	}controlType;

	void Init(GenericEntity* attach = nullptr);
	void Update(double dt);
	void Render();

	GenericEntity* attachedEntity;
	//rotation of the torso and leg, should make it a quaternion next time
	Vector3 torsoDirection;
	Vector3 legDirection;

	float moveDistance;
	float moveTimer;
	float speed;
	float rotationSpeed;
	float sensitivity;

private:
	Chassis chassis;

	//control for player
	void PlayerControl(double dt);
	//control for npc
	void NPCControl(double dt);
};

#endif 