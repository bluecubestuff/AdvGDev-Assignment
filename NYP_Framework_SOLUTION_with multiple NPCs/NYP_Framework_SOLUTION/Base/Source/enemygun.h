#pragma once

#include "WeaponInfo\WeaponInfo.h"

class CEnemyGun :
	public CWeaponInfo
{
public:
	CEnemyGun();
	virtual ~CEnemyGun();

	// Initialise this instance to default values
	void Init(void);
};

