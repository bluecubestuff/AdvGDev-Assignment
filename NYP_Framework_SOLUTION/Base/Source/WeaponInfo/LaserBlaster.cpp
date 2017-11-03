#include "LaserBlaster.h"
#include "../Projectile/Laser.h"

CLaserBlaster::CLaserBlaster()
{
}

CLaserBlaster::~CLaserBlaster()
{
}

void CLaserBlaster::Init()
{
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	magRounds = 100;
	// The maximum number of ammunition for this magazine for this weapon
	maxMagRounds = 100;
	// The current total number of rounds currently carried by this player
	totalRounds = 1000;
	// The max total number of rounds currently carried by this player
	maxTotalRounds = 1000;

	// The time between shots
	timeBetweenShots = 0.1333;
	// The elapsed time (between shots)
	elapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;
}

void CLaserBlaster::Discharge(Vector3 position, Vector3 target, CPlayerInfo * _source)
{
	if (bFire)
	{
		//if ammo is present
		if (magRounds > 0)
		{
			Vector3 _direction = (target - position).Normalize();
			//Create laser 
			CLaser* aLaser = Create::Laser("laser", position, _direction, 10.f, 2.f, 100.f, _source);

			aLaser->SetIsLaser(true);
			aLaser->SetCollider(false);
			aLaser->SetAABB(Vector3(0.5f, 0.5f, 0.5f), Vector3(-0.5f, -0.5f, 0.5f));
			bFire = false;
			--magRounds;
		}
	}
}
