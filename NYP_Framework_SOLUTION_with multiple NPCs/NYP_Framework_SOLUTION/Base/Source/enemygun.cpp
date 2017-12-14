#include "enemygun.h"


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
	magRounds = 100000000;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 100000000;
	// The current total number of rounds currently carried by this player
	totalRounds = 100000000;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 100000000;

	// The time between shots
	timeBetweenShots = 3;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
