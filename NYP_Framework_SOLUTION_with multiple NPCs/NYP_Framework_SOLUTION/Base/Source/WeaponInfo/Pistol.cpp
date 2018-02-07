#include "Pistol.h"
#include "LuaInterface.h"

CPistol::CPistol()
{
}


CPistol::~CPistol()
{
}

// Initialise this instance to default values
void CPistol::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = CLuaInterface::GetInstance()->getIntValue("pistol_magRounds");
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = CLuaInterface::GetInstance()->getIntValue("pistol_maxMagRounds");;
	// The current total number of rounds currently carried by this player
	totalRounds = CLuaInterface::GetInstance()->getIntValue("pistol_totalRounds");;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = CLuaInterface::GetInstance()->getIntValue("pistol_maxTotalRounds");;

	// The time between shots
	timeBetweenShots = CLuaInterface::GetInstance()->getFloatValue("pistol_timeBetweenShots");
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}
