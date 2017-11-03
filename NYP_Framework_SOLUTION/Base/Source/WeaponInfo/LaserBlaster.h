#pragma once

#include "WeaponInfo.h"

class CLaserBlaster : public CWeaponInfo
{
public:
	CLaserBlaster();
	virtual ~CLaserBlaster();

	//initialise this instance to default value
	void Init();
	// Discharge weapon
	void Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source = NULL);
};