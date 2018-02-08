#pragma once
#ifndef OPTION_H
#define OPTION_H

#include <vector>

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class Button;
class OptionState : public Scene
{
public:
	OptionState();
	~OptionState();

	float worldX;
	float worldY;
	std::vector<Button*> buttonList;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	static OptionState* me;

private:
	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	//	TextEntity* textObj[3];
};

#endif