#include "Mech.h"

#include "KeyboardController.h"
#include "MouseController.h"
#include "PlayerInfo\PlayerInfo.h"
#include "GenericEntity.h"
#include "Chassis.h"

void Mech::Init(GenericEntity* attach)
{
	torsoDirection.Set(1, 0, 0);
	legDirection.Set(1, 0, 0);

	moveDistance = 1.f;
	moveTimer = 1.f;
	speed = moveDistance / moveTimer;
	rotationSpeed = 45.f;
	sensitivity = 120.f;
	//if attach is nth, means that the user want to init as a player
	if (attach == nullptr) {
		controlType = PLAYER;
		position = CPlayerInfo::GetInstance()->GetPos();
	}
	else {
		controlType = NPC;
		attachedEntity = attach;
		position = attachedEntity->GetPosition();
	}
	chassis = new Chassis(this);
}

void Mech::Update(double dt)
{
	switch (controlType) {
	case PLAYER:
		PlayerControl(dt);
		break;
	case NPC:

		break;
	}
}

void Mech::Render()
{
}

void Mech::PlayerControl(double dt)
{
	double mouse_diff_x, mouse_diff_y;
	MouseController::GetInstance()->GetMouseDelta(mouse_diff_x, mouse_diff_y);

	double camera_yaw = mouse_diff_x * 0.0174555555555556;		// 3.142 / 180.0
	double camera_pitch = mouse_diff_y * 0.0174555555555556;	// 3.142 / 180.0

	//temp variables to make life easier
	Vector3 _pos = CPlayerInfo::GetInstance()->GetPos();
	Vector3 _tar = CPlayerInfo::GetInstance()->GetTarget();
																// Update the position if the WASD buttons were activated
	if (KeyboardController::GetInstance()->IsKeyDown('W') ||
		KeyboardController::GetInstance()->IsKeyDown('A') ||
		KeyboardController::GetInstance()->IsKeyDown('S') ||
		KeyboardController::GetInstance()->IsKeyDown('D'))
	{
		if (chassis->GetMovability()) {
			Vector3 viewVector = _tar - _pos;
			Vector3 rightUV;
			if (KeyboardController::GetInstance()->IsKeyDown('W'))
			{
				_pos += legDirection.Normalized() * speed;
				CPlayerInfo::GetInstance()->SetPos(_pos);
			}
			else if (KeyboardController::GetInstance()->IsKeyDown('S'))
			{
				_pos -= legDirection.Normalized() * speed;
				CPlayerInfo::GetInstance()->SetPos(_pos);
			}
			if (KeyboardController::GetInstance()->IsKeyDown('A'))
			{
				Mtx44 rotate;
				rotate.SetToRotation(rotationSpeed * dt, 0, 1, 0);
				legDirection = rotate * legDirection;
			}
			else if (KeyboardController::GetInstance()->IsKeyDown('D'))
			{
				Mtx44 rotate;
				rotate.SetToRotation(-rotationSpeed * dt, 0, 1, 0);
				legDirection = rotate * legDirection;
			}
			// Constrain the position
			//Constrain();
			// Update the target
			CPlayerInfo::GetInstance()->SetTarget(_pos + viewVector.Normalized());
			CPlayerInfo::GetInstance()->SetPos(_pos);

			//reset the temp variables
			_pos = CPlayerInfo::GetInstance()->GetPos();
			_tar = CPlayerInfo::GetInstance()->GetTarget();
		}
	}

	//Update the camera direction based on mouse move
	{
		Vector3 viewUV = (_tar - _pos).Normalized();
		Vector3 rightUV;
		{
			float yaw = (float)(-sensitivity * camera_yaw * (float)dt);
			Mtx44 rotation;
			rotation.SetToRotation(yaw, 0, 1, 0);
			viewUV = rotation * viewUV;
			_tar = _pos + viewUV;
			rightUV = viewUV.Cross(CPlayerInfo::GetInstance()->GetUp());
			rightUV.y = 0;
			rightUV.Normalize();
			CPlayerInfo::GetInstance()->SetUp(rightUV.Cross(viewUV).Normalized());
		}
		{
			float pitch = (float)(-sensitivity * camera_pitch * (float)dt);
			rightUV = viewUV.Cross(CPlayerInfo::GetInstance()->GetUp());
			rightUV.y = 0;
			rightUV.Normalize();
			CPlayerInfo::GetInstance()->SetUp(rightUV.Cross(viewUV).Normalized());
			Mtx44 rotation;
			rotation.SetToRotation(pitch, rightUV.x, rightUV.y, rightUV.z);
			viewUV = rotation * viewUV;
			_tar = _pos + viewUV;
		}
	}
	CPlayerInfo::GetInstance()->SetTarget(_tar);

	//update the mech position
	position = CPlayerInfo::GetInstance()->GetPos();
	//update chassis
	chassis->Update(dt);
}

void Mech::NPCControl(double dt)
{
}
