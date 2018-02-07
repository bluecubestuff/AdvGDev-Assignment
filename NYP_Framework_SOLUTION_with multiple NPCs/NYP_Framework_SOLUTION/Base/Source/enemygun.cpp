#include "enemygun.h"
#include "LuaInterface.h"

CEnemyGun::CEnemyGun()
{
}


CEnemyGun::~CEnemyGun()
{
}

// Initialise this instance to default values
void CEnemyGun::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = CLuaInterface::GetInstance()->getIntValue("EG_magRounds");
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = CLuaInterface::GetInstance()->getIntValue("EG_maxMagRounds");
	// The current total number of rounds currently carried by this player
	totalRounds = CLuaInterface::GetInstance()->getIntValue("EG_totalRounds");
	// The max total number of rounds currently carried by this player
	maxTotalRounds = CLuaInterface::GetInstance()->getIntValue("EG_maxTotalRounds");

	// The time between shots
	timeBetweenShots = CLuaInterface::GetInstance()->getFloatValue("EG_timeBetweenShots");
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
