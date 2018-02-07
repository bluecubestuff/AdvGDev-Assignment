#include "GrenadeThrow.h"
#include "../Projectile/Grenade.h"
#include "LuaInterface.h"

CGrenadeThrow::CGrenadeThrow()
{
}


CGrenadeThrow::~CGrenadeThrow()
{
}

// Initialise this instance to default values
void CGrenadeThrow::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = CLuaInterface::GetInstance()->getIntValue("grenade_magRounds");
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = CLuaInterface::GetInstance()->getIntValue("grenade_maxMagRounds");
	// The current total number of rounds currently carried by this player
	totalRounds = CLuaInterface::GetInstance()->getIntValue("grenade_totalRounds");
	// The max total number of rounds currently carried by this player
	maxTotalRounds = CLuaInterface::GetInstance()->getIntValue("grenade_maxTotalRounds");

	// The time between shots
	timeBetweenShots = CLuaInterface::GetInstance()->getFloatValue("grenade_timeBetweenShots");
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

// Discharge this weapon
void CGrenadeThrow::Discharge(Vector3 position, Vector3 target, CPlayerInfo* _source)
{
	if (bFire)
	{
		// If there is still ammo in the magazine, then fire
		if (magRounds > 0)
		{
			// Create a projectile with a cube mesh. Its position and direction is same as the player.
			// It will last for 3.0 seconds and travel at 500 units per second
			CProjectile* aProjectile = Create::Grenade("sphere",
														position,
														(target - position).Normalized(),
														5.0f,
														8.0f,
														_source);
			aProjectile->SetCollider(true);
			aProjectile->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, -0.5f));
			bFire = true;
			magRounds--;
		}
	}
}