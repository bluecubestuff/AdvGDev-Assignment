#ifndef CHASSIS_H
#define CHASSIS_H

class CSceneNode;
class Mech;
class Torso;
class Arm;
class Leg;
class Head;

//class to move everything tgt, respond to controls
class Chassis {
public:
	Chassis(Mech* parent);
	~Chassis();

	//check if mech is able to move
	bool GetMovability();

	void Update(double dt);

	//the chassis is attached to a mech, will update according to the mech
	Mech* parent;
private:
	Torso* torso;
	CSceneNode* torsoNode;
	Leg* leftLeg;
	Leg* rightLeg;
};

#endif