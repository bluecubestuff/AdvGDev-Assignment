#ifndef CHASSIS_H
#define CHASSIS_H

class Mech;

//class to move everything tgt, respond to controls
class Chassis {
public:
	Chassis();
	~Chassis();

private:
	//the chassis is attached to a mech, will update according to the mech
	Mech* parent;
};

#endif